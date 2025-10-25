#include "soldier.h"

#include <game/server/entities/character.h>
#include <game/server/gamecontext.h>

const char *CBlockClassSoldier::CommandName() const
{
	return "soldier";
}

const char *CBlockClassSoldier::DisplayName() const
{
	return "Soldier";
}

const char *CBlockClassSoldier::Description() const
{
	return "Granadas congelam o alvo e arremessam para longe.";
}

void CBlockClassSoldier::OnExplosionHit(CGameContext *pGameServer, int Weapon, CCharacter *pOwner, CCharacter *pTarget, vec2 HitPos, vec2 ForceDir)
{
	(void)HitPos;

	if(!pGameServer || Weapon != WEAPON_GRENADE || !pOwner || !pTarget)
	{
		return;
	}

	if(pOwner == pTarget)
	{
		return;
	}

	if(!pTarget->IsAlive())
	{
		return;
	}

	// Freeze the enemy and launch them away from the blast.
	// pTarget->Freeze(m_FreezeSeconds);

	vec2 Dir = ForceDir;
	if(length(Dir) <= 0.0001f)
	{
		Dir = pTarget->Core()->m_Pos - pOwner->Core()->m_Pos;
	}
	if(length(Dir) <= 0.0001f)
	{
		Dir = vec2(0.0f, -1.0f);
	}
	Dir = normalize(Dir);
	pTarget->SetVelocity(vec2(0.0f, 0.0f));
	pTarget->AddVelocity(Dir * m_Knockback);
}
