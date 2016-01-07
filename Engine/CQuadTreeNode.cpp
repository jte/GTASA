#include "StdInc.h"

int CQuadTreeNode::FindSector(const CRect& rect)
{
    if (!m_numFreeNodes)
    {
        return -1;
    }

    float halfx = (m_sector.left + m_sector.right) / 2.0f;
    float halfy = (m_sector.top + m_sector.bottom) / 2.0f;

    //          halfx
    //           | |
    //     II    | |   I
    //  =========|C|========== halfy
    //     III   | |  IV
    //           | |

    /* sector in center of grid */
    if (rect.top >= halfy && rect.bottom <= halfy)
    {
        return -1;
    }
    /* II quadrant */
    if (rect.top >= halfy && rect.right < halfx)
    {
        return 0;
    }
    /* I quadrant */
    if (rect.top >= halfy && rect.left > halfx)
    {
        return 1;
    }
    /* III quadrant */
    if (rect.top < halfy && rect.right < halfx)
    {
        return 2;
    }
    /* IV quadrant */
    if (rect.top < halfy && rect.left > halfx)
    {
        return 3;
    }
    return -1;
}

int CQuadTreeNode::FindSector(const CVector2D& vector)
{
    if (!m_uiChildrenUnusedNodeCount)
    {
        return -1;
    }
    float halfx = (m_sector.left + m_sector.right) / 2.0f;
    float halfy = (m_sector.top + m_sector.bottom) / 2.0f;
    /* I quadrant */
    if (vector.x > halfx && vector.y >= halfy)
    {
        return 1;
    }
    /* IV quadrant */
    if (vector.x > halfx && vector.y < halfy)
    {
        return 3;
    }
    /* II quadrant */
    if (vector.x < halfx && vector.y >= halfy)
    {
        return 0;
    }
    /* III quadrant*/
    if (vector.x < halfx && vector.y < halfy)
    {
        return 2;
    }
    return -1;
}

bool CQuadTreeNode::InSector(const CRect& rect, int quadrant)
{
    if (!m_numFreeNodes)
    {
        return false;
    }

    float left = m_sector.left;
    float top = m_sector.top;
    float bottom = m_sector.bottom;
    float right = m_sector.right;
    float v3;
    switch ( quadrant )
    {
    case 0:
        v3 = left;
        right = (right + left) * 0.5;
        bottom = (bottom + top) * 0.5;
    case 1:
        v3 = (left + right) * 0.5;
        bottom = (bottom + top) * 0.5;
        break;
    case 2:
        v3 = left;
        right = (right + left) * 0.5;
        top = (bottom + top) * 0.5;
    case 3:
        v3 = (left + right) * 0.5;
        top = (bottom + top) * 0.5;
        break;
    default:
        v3 = left;
        break;
    }
    if ( v3 > rect.right )
        return 0;
    return right >= rect.left && bottom <= rect.top && top >= rect.bottom;
}

void CQuadTreeNode::GetAllMatching(const CRect& sector, CPtrListSingleLink& list)
{
    for (CPtrNodeSingleLink* cur = m_itemList->head; cur; cur = cur->next)
    {
        list.Add(cur->ptr);
    }

    for (size_t i = 0; i < ELEMS_COUNT(m_childrens); i++)
    {
        if (m_childrens[i] && InSector(sector, i))
        {
            m_childrens[i]->GetAllMatching(sector, list);
        }
    }
}

void CQuadTreeNode::GetAllMatching(const CVector2D& sector, CPtrListSingleLink& list)
{
    for (CPtrNodeSingleLink* cur = m_itemList->head; cur; cur = cur->next)
    {
        list.Add(cur->ptr);
    }

    int childSector = FindSector(sector);
    if (childSector != -1 && m_childrens[childSector])
    {
        m_childrens[childSector]->GetAllMatching(sector, list);
    }
}

void CQuadTreeNode::ForAllMatching(const CRect& sector, RectSectorMatchingCB callback)
{
    for (CPtrNodeSingleLink* cur = m_itemList->head; cur; cur = cur->next)
    {
        callback(sector, cur->ptr);
    }

    for (size_t i = 0; i < ELEMS_COUNT(m_childrens); i++)
    {
        if (InSector(sector, i))
        {
            m_childrens[i]->ForAllMatching(sector, callback);
        }
    }
}

void CQuadTreeNode::ForAllMatching(const CVector2D& sector, Vec2dSectorMatchingCB callback)
{
    for (CPtrNodeSingleLink* cur = m_itemList->head; cur; cur = cur->next)
    {
        callback(sector, cur->ptr);
    }

    int childSector = FindSector(sector);
    if (childSector != -1 && m_childrens[childSector])
    {
        m_childrens[childSector]->ForAllMatching(sector, callback);
    }
}

void CQuadTreeNode::AddItem(void* item, const CRect& sector)
{
    if (m_numFreeNodes == 0)
    {
        m_itemList->Add(item);
    }
    for (size_t i = 0; i < ELEMS_COUNT(m_childrens); i++)
    {
        if (!InSector(sector, i))
        {
            continue;
        }
        if (!m_childrens[i])
        {
            CRect sector = m_sector;
            switch (i)
            {
                case 0:
                    sector.right = (sector.left + sector.right) * 0.5;
                    sector.bottom = (sector.bottom + sector.top) * 0.5;
                break;
                case 1:
                    sector.left = (sector.left + sector.right) * 0.5;
                    sector.bottom = (sector.bottom + sector.top) * 0.5;
                break;
                case 2:
                    sector.right = (sector.left + sector.right) * 0.5;
                    sector.top = (sector.bottom + sector.top) * 0.5;
                break;
                case 3:
                    sector.left = (sector.left + sector.right) * 0.5;
                    sector.top = (sector.bottom + sector.top) * 0.5;
                break;
                default:
                break;
            }
            m_childrens[i] = new CQuadTreeNode(sector, m_numFreeNodes - 1);
        }
        m_childrens[i]->AddItem(item, sector);
    }
}

CQuadTreeNode::CQuadTreeNode(const CRect& sector, int numFreeNodes)
{
    m_sector = sector;
    m_itemList = NULL;
    m_numFreeNodes = numFreeNodes;
    memset(m_childrens, 0, sizeof(m_childrens));
}