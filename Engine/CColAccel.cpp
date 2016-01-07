#include "StdInc.h"

const char CColAccel::mp_cCacheName[] = "MODELS\\CINFO.BIN";

bool CColAccel::isCacheLoading()
{
    return m_iCacheState == 2;
}

IplDef CColAccel::getIplDef(size_t id)
{
    return m_iplDefs[id];
}

void CColAccel::endCache()
{
	if (m_iCacheState == 1)
	{
		int fp = CFileMgr::OpenFileForWriting(mp_cCacheName);
		CFileMgr::Write(fp, (char*)&m_iNumColItems, sizeof(m_iNumColItems));
		CFileMgr::Write(fp, mp_caccColItems, 0x30 * m_iNumColItems);
		CFileMgr::Write(fp, (char*)&m_iNumSections, sizeof(m_iNumSections));
		CFileMgr::Write(fp, (char*)m_iSectionSize, 4 * m_iNumSections);
		CFileMgr::Write(fp, (char*)m_iplDefs, sizeof(IplDef) * 256);
		CFileMgr::Write(fp, (char*)&m_iNumColBounds, sizeof(m_iNumColBounds));
		CFileMgr::Write(fp, CColAccel::m_colBounds, 24 * m_iNumColBounds);
		CFileMgr::Write(fp, (char*)&m_iNumIPLItems, sizeof(m_iNumIPLItems));
		CFileMgr::Write(fp, CColAccel::mp_caccIPLItems, 20 * CColAccel::m_iNumIPLItems);
		CFileMgr::CloseFile(fp);
	}
  if (mp_caccIPLItems)
  {
		delete(mp_caccIPLItems);
		mp_caccIPLItems = 0;
  }
  if ( CColAccel::mp_caccColItems )
  {
    delete(CColAccel::mp_caccColItems);
    CColAccel::mp_caccColItems = 0;
  }
  delete(CColAccel::m_iSectionSize);
  CColAccel::m_iSectionSize = 0;
  delete(CColAccel::m_iplDefs);
  CColAccel::m_iplDefs = 0;
  delete(CColAccel::m_colBounds);
  CColAccel::m_colBounds = 0;
  CColAccel::m_iCacheState = 0;
}

void CColAccel::addCacheCol(int modelId, const CColModel& colModel)
{
	if (m_iCacheState == 1)
	{
		mp_caccColItems[m_iNumColItems].modelId = modelId;
		mp_caccColItems[m_iNumColItems].boundingBox = colModel.GetBoundingBox();
		mp_caccColItems[m_iNumColItems].colNum = colModel.m_colNum;
		mp_caccColItems[m_iNumColItems].colModelFlag = colModel.flags & 1;
		m_iNumColItems++;
	}
}

void CColAccel::cacheLoadCol()
{
	if (m_iCacheState == 2)
	{
		for (size_t i = 0; i < m_iNumColItems; i++)
		{
			CBaseModelInfo* modelInfo = CModelInfo::GetModelInfo(mp_caccColItems[i].modelId);
			CColModel* colModel = new CColModel;
			colModel->SetBoundingBox(mp_caccColItems[i].boundingBox);
			colModel->SetColNum(mp_caccColItems[i].colNum);
			colModel->flags = mp_caccColItems[i].colModelFlag;
			modelInfo->SetColModel(colModel, true);
			CColStore::IncludeModelIndex(mp_caccColItems[i].colNum, mp_caccColItems[i].modelId);
		}
	}
}