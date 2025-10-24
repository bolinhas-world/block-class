#include "ninja.h"

#include <game/server/entities/character.h>
#include <game/server/gamecontext.h>

const char *CBlockClassNinja::CommandName() const
{
	return "ninja";
}

const char *CBlockClassNinja::DisplayName() const
{
	return "Ninja";
}

const char *CBlockClassNinja::Description() const
{
	return "Movimenta-se muito mais rapido que o normal.";
}

void CBlockClassNinja::OnCharacterTick(CGameContext *pGameServer, CCharacter *pCharacter)
{
	(void)pGameServer;
	if(!pCharacter)
	{
		return;
	}

	CCharacterCore Core = pCharacter->GetCore();
	const float BaseSpeed = (float)Core.m_Tuning.m_GroundControlSpeed;
	const float BaseAccel = (float)Core.m_Tuning.m_GroundControlAccel;
	Core.m_Tuning.m_GroundControlSpeed = BaseSpeed * m_GroundSpeedMultiplier;
	Core.m_Tuning.m_GroundControlAccel = BaseAccel * m_GroundAccelMultiplier;
	pCharacter->SetCore(Core);
}
