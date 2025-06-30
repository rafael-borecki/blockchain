import type { Block } from "./Dashboard";
import ReactDOM from 'react-dom';
import CloseIcon from '@mui/icons-material/Close';

interface BlockDataPopUpProps {
    block: Block;
    setVisibility: React.Dispatch<React.SetStateAction<boolean>>;
}


function renderField(label: string, value: string | number) {
    return (
        <div className="flex items-center gap-2">
            <span className="font-bold">{label}:</span>
            <pre className="bg-gray-100 p-2 rounded text-sm overflow-x-auto whitespace-pre-wrap"> 
                {value}
            </pre>
        </div>
    );
}


function BlockDataPopUp({ block, setVisibility }: BlockDataPopUpProps) {
    return ReactDOM.createPortal(
        <div className="absolute top-0 left-0 w-full h-full bg-[rgba(0,0,0,0.5)] flex justify-center items-center">
            <div className="bg-white p-6 rounded-lg shadow-lg w-fit flex flex-col gap-2">
                <div className="flex justify-between items-center">
                    <p className="text-2xl font-bold">{`Block ${block.height}`}</p>
                    <CloseIcon
                        className="cursor-pointer"
                        onClick={() => setVisibility(false)}
                    />
                </div>
                {renderField('Timestamp', block.timestamp)}
                {renderField('Hash Meta', block.hashMeta)}
                {renderField('Previous Block Hash', block.prevBlockHash)}
                {renderField('Block Hash', block.blockHash)}
                {renderField('Nonce', block.nonce)}
                {renderField('Full Block Hash', block.fullBlockHash)}
                {renderField('Height', block.height)}
                {renderField('Data', (() => {
                    try {
                        const parsedData = JSON.parse(block.data);
                        return JSON.stringify(parsedData, null, 2);
                    } catch {
                        return block.data;
                    }
                })())}
            </div>

        </div>,
        document.body
    )
}

export default BlockDataPopUp;