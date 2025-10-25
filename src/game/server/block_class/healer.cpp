#include "healer.h"

#include <generated/protocol.h>

#include <game/server/entities/character.h>
#include <game/server/gamecontext.h>
#include <game/server/player.h>

const char *CBlockClassHealer::CommandName() const
{
	return "healer";
}

const char *CBlockClassHealer::DisplayName() const
{
	return "Healer";
}

const char *CBlockClassHealer::Description() const
{
	return "A pistola descongela alvos congelados, mas atira mais devagar.";
}

float CBlockClassHealer::AdjustWeaponFireDelay(CGameContext *pGameServer, CCharacter *pCharacter, int Weapon, float BaseDelay) const
{
	(void)pGameServer;
	(void)pCharacter;

	if(Weapon != WEAPON_GUN)
	{
		return BaseDelay;
	}

	return BaseDelay * m_GunFireDelayMultiplier;
}

void CBlockClassHealer::OnProjectileHit(CGameContext *pGameServer, int Weapon, CCharacter *pOwner, CCharacter *pTarget, vec2 HitPos)
{
	(void)pOwner;

	if(Weapon != WEAPON_GUN || !pTarget)
	{
		return;
	}

	if(!pTarget->UnFreeze())
	{
		return;
	}

	if(pGameServer)
	{
		pGameServer->CreateSound(HitPos, SOUND_PICKUP_HEALTH, pTarget->TeamMask());
	}
}

void CBlockClassHealer::OnCharacterTakeDamage(CGameContext *pGameServer, CCharacter *pCharacter, vec2 &Force, int &Damage, int From, int Weapon)
{
	(void)pGameServer;
	(void)pCharacter;
	(void)Damage;
	(void)From;
	(void)Weapon;

	if(Force.x != 0.0f || Force.y != 0.0f)
	{
		Force *= m_KnockbackMultiplier;
	}
}
