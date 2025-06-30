import { StrictMode } from 'react'
import { createRoot } from 'react-dom/client'
import './index.css'
import Dashboard from './Dashboard'
import { ReactFlowProvider } from '@xyflow/react'

createRoot(document.getElementById('root')!).render(
  <StrictMode>
    <ReactFlowProvider>
      <Dashboard />
    </ReactFlowProvider>
  </StrictMode>,
)
