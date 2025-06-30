import FlowCanvas from "./FlowCanvas"
import { useState, useEffect } from "react";

const API_URL = "/api";

export interface Block {
    height: number;
    timestamp: number;
    hashMeta: string;
    prevBlockHash: string;
    data: string;
    blockHash: string;
    nonce: string;
    fullBlockHash: string;
}

async function getBlocks(size: number): Promise<Block[]> {
    const response = await fetch(`${API_URL}/blocks?n_blocks=${size}`);
    if (!response.ok) {
        console.error("Failed to fetch blocks:", response.statusText);
    }
    const data = await response.json();
    return data;

}

function removeWhitespace(input: string): string {
    return input.replace(/\s+/g, '');
}

function Dashboard() {
    const [blocks, setBlocks] = useState<Block[]>([]);
    const [blockData, setBlockData] = useState<string>("");


    useEffect(() => {
        getBlocks(200).then(fetchedBlocks => {
            setBlocks(fetchedBlocks);
        });
    }, []);

    useEffect(() => {
        const es = new EventSource(`${API_URL}/blocks/stream`);

        es.onmessage = (event) => {
            try {
                console.log("Received SSE data:", event.data);
                const newBlock: Block = JSON.parse(event.data);

                setBlocks((prev) =>
                    prev.some((b) => b.height === newBlock.height)
                        ? prev
                        : [newBlock, ...prev]
                );
            } catch (err) {
                console.error("SSE parse error:", err);
            }
        };

        es.onerror = (err) => {
            console.error("SSE connection error:", err);
        };

        return () => {
            es.close();
        };
    }, []);

    async function handleNewBlock() {
        if (!blockData) {
            alert("Please enter block data.");
            return;
        }
        try {
            const response = await fetch(`${API_URL}/data`, {
                method: "POST",
                headers: {
                    "Content-Type": "application/json",
                },
                body: removeWhitespace(blockData),
            });

            if (!response.ok) {
                throw new Error("Failed to submit block");
            }

            setBlockData("");
        } catch (error) {
            console.error("Error submitting block:", error);
            alert("Failed to submit block. Please check the console for details.");
        }
    }


    return (
        <div className="flex h-screen w-screen flex-col justify-center items-center bg-slate-50 gap-6">
            <h1 className="text-2xl font-bold mb-4">Blockchain Dashboard</h1>
            <div className="h-[50vh] w-[95vw] border-2 border-gray-300 rounded-lg p-4 bg-white shadow">
                <FlowCanvas
                    blocks={blocks}
                />
            </div>
            <div className="h-[30vh] w-[95vw] border-2 border-gray-300 rounded-lg p-4 bg-white shadow">
                <p className="font-bold text-2xl w-full flex justify-center">Insert new block</p>
                <div className="flex flex-col gap-2 items-center">
                    <label className="font-bold w-full">Block Data:</label>
                    <textarea
                        className="rounded-lg p-2 w-full resize-none outline-none border-2 "
                        rows={4}
                        placeholder="Enter block data here..."
                        value={(() => {
                            try {
                                const parsed = JSON.parse(blockData);
                                return JSON.stringify(parsed, null, 2);
                            } catch {
                                return blockData;
                            }
                        })()}
                        onChange={(e) => {
                            try {
                                const parsed = JSON.parse(e.target.value);
                                setBlockData(JSON.stringify(parsed));
                            } catch {
                                setBlockData(e.target.value);
                            }
                        }}
                    />
                    <button
                        className="bg-amber-500 w-fit font-bold px-4 py-2 rounded hover:bg-amber-400 cursor-pointer transition-all duration-300 active:scale-95"
                        onClick={handleNewBlock}
                    >
                        Submit Block
                    </button>
                </div>
            </div>
        </div>
    )
}

export default Dashboard;