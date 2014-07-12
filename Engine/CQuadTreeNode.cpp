#include "StdInc.h"

void CQuadTreeNode::AddItem(void *pItem, CRect const &sector)
{
    if(m_uiChildrenUnusedNodeCount)
    {
        CRect lsector = sector;
        for(size_t i = 0; i < sizeof(m_pChildrens); i++)
        {
            if(InSector(sector, i))
            {
                if(!m_pChildrens[i])
                {
                    lsector = m_sector;
                    switch(i)
                    {
                    case 0:
                        lsector.right = (sector.left + sector.right) * 0.5;
                        lsector.bottom = (sector.bottom + sector.top) * 0.5;
                    break;
                    case 1:
                        lsector.left = (sector.left + sector.right) * 0.5;
                        lsector.bottom = (sector.bottom + sector.top) * 0.5;
                    break;
                    case 2:
                        lsector.right = (sector.left + sector.right) * 0.5;
                        lsector.top = (sector.bottom + sector.top) * 0.5;
                    break;
                    case 3:
                        lsector.left = (sector.left + sector.right) * 0.5;
                        lsector.top = (sector.bottom + sector.top) * 0.5;
                    break;
                    }
                    CQuadTreeNode *pNode = new CQuadTreeNode;
                    if(pNode)
                    {
                        pNode->m_sector = lsector;
                        pNode->m_pItemList = NULL;
                        pNode->m_uiChildrenUnusedNodeCount = m_uiChildrenUnusedNodeCount - 1;
                        memset(pNode->m_pChildrens, 0, sizeof(pNode->m_pChildrens));
                    }
                    m_pChildrens[i] = pNode;
                }
                m_pChildrens[i]->AddItem(pItem, lsector);
            }
        }
    }
    else
    {
        CPtrNodeSingleLink *pNode = new CPtrNodeSingleLink;
        if(pNode)
        {
            pNode->pPtr = pItem;
            pNode->pNext = m_pItemList;
            m_pItemList = pNode;
        }
    }
}