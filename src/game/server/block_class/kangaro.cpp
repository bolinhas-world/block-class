#include "kangaro.h"

#include <generated/protocol.h>

#include <game/server/entities/character.h>
#include <game/server/entities/projectile.h>
#include <game/server/gamecontext.h>
#include <game/server/player.h>

const char *CBlockClassKangaro::CommandName() const
{
	return "kangaro";
}

const char *CBlockClassKangaro::DisplayName() const
{
	return "Kangaro";
}

const char *CBlockClassKangaro::Description() const
{
	return "A pistola dispara um foguete que explode logo apos sair do cano.";
}

float CBlockClassKangaro::AdjustWeaponFireDelay(CGameContext *pGameServer, CCharacter *pCharacter, int Weapon, float BaseDelay) const
{
	(void)pGameServer;
	(void)pCharacter;

	if(Weapon != WEAPON_GUN)
	{
		return BaseDelay;
	}

	return m_GunFireDelayMs;
}

bool CBlockClassKangaro::OnCharacterGunFire(CGameContext *pGameServer, CCharacter *pCharacter, vec2 ProjStartPos, vec2 Direction, vec2 MouseTarget)
{
	(void)MouseTarget;
	if(!pGameServer || !pCharacter)
	{
		return false;
	}

	CPlayer *pPlayer = pCharacter->GetPlayer();
	if(!pPlayer)
	{
		return false;
	}

	const int OwnerId = pPlayer->GetCid();
	if(OwnerId < 0 || OwnerId >= MAX_CLIENTS)
	{
		return false;
	}

	vec2 AimDir = Direction;
	if(length(AimDir) < 0.0001f)
	{
		AimDir = vec2(0.0f, -1.0f);
	}
	else
	{
		AimDir = normalize(AimDir);
	}

	const float ExplosionOffset = 32.0f;
	const vec2 ExplosionPos = ProjStartPos + AimDir * ExplosionOffset;

	pGameServer->CreateSound(pCharacter->m_Pos, SOUND_GRENADE_FIRE, pCharacter->TeamMask());
	pGameServer->CreateExplosion(ExplosionPos, OwnerId, WEAPON_GRENADE, false, pCharacter->Team(), pCharacter->TeamMask());
	return true;
}
