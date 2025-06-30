from fastapi import FastAPI, Request, status, HTTPException
from fastapi.responses import StreamingResponse, JSONResponse
import json
from app.kafka import init_kafka, stop_kafka, send_raw, blocks_stream
from app.file_reader import read_blockchain
from fastapi.middleware.cors import CORSMiddleware

app = FastAPI(title="Kafka-Bridge", root_path="/api", debug=True)

app.add_middleware(
    CORSMiddleware,
    allow_origins=["*"],
    allow_credentials=True,
    allow_methods=["*"],
    allow_headers=["*"],
)

@app.on_event("startup")
async def startup():
    await init_kafka()

@app.on_event("shutdown")
async def shutdown():
    await stop_kafka()


'''
SSE para receber blocos minerados em tempo real.
'''
@app.get("/blocks/stream")
async def sse_blocks():
    return StreamingResponse(
        blocks_stream(),
        media_type="text/event-stream",
        headers={
            "Cache-Control": "no-cache",
            "Access-Control-Allow-Origin": "*",
            "X-Accel-Buffering": "no"
        }
    )


'''
adicona um bloco com os dados do body
'''
@app.post("/data", status_code=status.HTTP_202_ACCEPTED)
async def post_data(request: Request):
    try:
        payload = await request.body()
    except json.JSONDecodeError:
        raise HTTPException(status_code=400, detail="JSON inválido")


    await send_raw(payload.decode("utf-8"))
    return JSONResponse({"status": "queued"})

'''
retorna:
[{
    "height": int,
    "timestamp": int,
    "hashMeta": str,
    "prevBlockHash": str,
    "data": str,
    "blockHash": str,
    "nonce": str,
    "fullBlockHash": str
}, ...]
'''
@app.get("/blocks")
async def get_blocks(n_blocks: int = -1):
    try:
        blocks = read_blockchain('./out/output.bin', n_blocks=n_blocks)
    except (IOError, ValueError) as e:
        raise HTTPException(status_code=500, detail=str(e))
    return JSONResponse(blocks)