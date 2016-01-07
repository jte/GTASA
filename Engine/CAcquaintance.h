#pragma once

class CAcquaintance
{
public:
	CAcquaintance();
	/* ClearAcquaintances(int)
	* ClearAsAcquaintance(int, unsigned int)
	* GetAcquaintances(int) const
	* SetAcquaintances(int, unsigned int)
	* SetAsAcquaintance(int, unsigned int)
	*/
private:
	uint32_t m_respect;
	uint32_t m_like;
	uint32_t m_pad;
	uint32_t m_dislike;
	uint32_t m_hate;
};