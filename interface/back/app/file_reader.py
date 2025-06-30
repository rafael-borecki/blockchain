import time
from typing import Dict, List, Union

def read_blockchain(
    filename: str,
    n_blocks: int = -1,
    *,
    max_retries: int = 5,
    retry_delay: float = 0.5,
    header_size: int = 16,
    block_size: int = 512,
) -> List[Dict[str, Union[int, str]]]:
    attempt = 0
    while True:
        try:
            with open(filename, "rb") as f:
                header = f.read(header_size)
                if len(header) < header_size:
                    raise ValueError("Header menor que 16 bytes")

                if header[0:1] != b"0":
                    raise IOError("Arquivo ainda está aberto em escrita")

                block_count = int(header[1:].split(b"\0", 1)[0] or b"0")

                if n_blocks in (-1, None):
                    n_fetch = block_count
                elif n_blocks >= 0:
                    n_fetch = min(n_blocks, block_count)
                else:
                    raise ValueError("n_blocks deve ser -1, None ou >= 0")

                start_idx = block_count - n_fetch
                f.seek(header_size + start_idx * block_size)

                blocks: List[Dict[str, Union[int, str]]] = []
                for _ in range(n_fetch):
                    raw = f.read(block_size)
                    if len(raw) < block_size:
                        raise ValueError("Final inesperado do arquivo")

                    fields = raw.split(b"\0", 8)[:8]
                    (height, ts, hash_meta, prev_hash,
                     data, block_hash, nonce, full_hash) = (
                        fld.decode("utf-8") for fld in fields
                    )

                    blocks.append({
                        "height": height,
                        "timestamp": ts,
                        "hashMeta": hash_meta,
                        "prevBlockHash": prev_hash,
                        "data": data,
                        "blockHash": block_hash,
                        "nonce": nonce,
                        "fullBlockHash": full_hash,
                    })
            blocks.reverse()
            return blocks

        except (FileNotFoundError, IOError, OSError) as exc:
            attempt += 1
            if attempt >= max_retries:
                raise
            time.sleep(retry_delay)
