import { type Edge, type Node } from '@xyflow/react';
import { useReactFlow, ReactFlow, useNodesInitialized } from '@xyflow/react';
import ChainNode from './ChainNode';
import '@xyflow/react/dist/style.css';
import { type Block } from './Dashboard';
import { useEffect, useState } from 'react';


const nodeTypes = { chainNode: ChainNode };


export default function FlowCanvas({ blocks }: { blocks: Block[] }) {
  const [nodes, setNodes] = useState<Node[]>([]);
  const [edges, setEdges] = useState<Edge[]>([]);
  const nodesInitialized = useNodesInitialized();
  const { setViewport } = useReactFlow();

  

  useEffect(() => {
    const newNodes: Node[] = blocks.map((block, index) => ({
      id: block.blockHash,
      position: { x: 300 * (blocks.length - index), y: 0 },
      data: { block },
      type: 'chainNode',
    }));

    const newEdges: Edge[] = blocks.slice(1).map((block, index) => ({
      id: `e${blocks[index].blockHash}-${block.blockHash}`,
      source: blocks[index].blockHash,
      target: block.blockHash,
      sourceHandle: 'source',
    }));

    setNodes(newNodes);
    setEdges(newEdges);
  }, [blocks]);

  useEffect(() => {
    if (nodes.length > 0) {
      const lastNode = nodes[0];
      const canvasWidth = window.innerWidth;
      const paddingRight = 300;

      const desiredX = canvasWidth - lastNode.position.x - 200 - paddingRight;

      setViewport({
        x: desiredX,
        y: 120,
        zoom: 1,
      });
    }
  }, [nodesInitialized, nodes, setViewport]);

  return (
      <ReactFlow
        nodes={nodes}
        edges={edges}
        nodeTypes={nodeTypes}
        nodesConnectable={false}
        proOptions={{ hideAttribution: true }}
        minZoom={0.05}
        fitView={false}
      />
  );
}
