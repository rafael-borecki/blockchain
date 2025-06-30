import { Handle, Position } from '@xyflow/react';
import { type Block } from './Dashboard';
import { format } from 'date-fns';
import BlockDataPopUp from './BlockDataPopUp';
import { useState } from 'react';

interface ChainNodeProps {
    data: {
        block: Block;
    }
}

function convertTimestampToDate(timestamp: number): string {
    try {
        return format(new Date(timestamp), "dd/MM/yyyy HH:mm:ss");
    } catch {
        return timestamp.toString();
    }
}

function ChainNode({ data }: ChainNodeProps) {
    const [showPopup, setShowPopup] = useState(false);

    return (
        <>
            {showPopup && <BlockDataPopUp block={data.block} setVisibility={setShowPopup} />}
            <div className="bg-amber-400 p-4 rounded-lg shadow-md w-64 cursor-pointer flex flex-col gap-2" onClick={() => setShowPopup(true)}>
                <div className='w-full flex justify-center font-bold text-2xl'>
                    {data.block.height}
                </div>
                <div className='flex gap-1'>
                    <div className='font-bold'>Date:</div>
                    {convertTimestampToDate(data.block.timestamp)}
                </div>
                <pre className='w-full bg-white rounded p-2 whitespace-pre-wrap overflow-x-auto'>
                    {(() => {
                        try {
                            const parsed = JSON.parse(data.block.data);
                            return JSON.stringify(parsed, null, 2);
                        } catch {
                            return data.block.data;
                        }
                    })()}
                </pre>
                <Handle type="source" position={Position.Left} id="source" />
                <Handle type="target" position={Position.Right} />
            </div>
        </>
    );
}

export default ChainNode;