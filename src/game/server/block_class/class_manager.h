#ifndef GAME_SERVER_BLOCK_CLASS_CLASS_MANAGER_H
#define GAME_SERVER_BLOCK_CLASS_CLASS_MANAGER_H

#include "block_class.h"
#include <array>
#include <memory>

#include <engine/shared/protocol.h>

class CBlockClassManager
{
public:
	explicit CBlockClassManager(CGameContext *pGameServer);

	void ResetPlayer(int ClientId);
	void HandlePlayerDisconnect(int ClientId);

	void OnCharacterSpawn(CCharacter *pCharacter);
	void OnCharacterTick(CCharacter *pCharacter);
	void OnExplosionHit(int OwnerId, int Weapon, CCharacter *pOwner, CCharacter *pTarget, vec2 HitPos, vec2 ForceDir);
	void OnProjectileHit(int OwnerId, int Weapon, CCharacter *pOwner, CCharacter *pTarget, vec2 HitPos);
	float AdjustWeaponFireDelay(int ClientId, CCharacter *pCharacter, int Weapon, float BaseDelay) const;

	void ListClasses(int ClientId) const;
	bool HandleClassCommand(int ClientId, const char *pClassName);
	bool HandleHelpRequest(int ClientId, const char *pQuery) const;

private:
	enum class EClassId : int
	{
		Soldier = 0,
		Runner,
		Healer,
		COUNT
	};

	static constexpr int INVALID_CLASS = -1;

	CGameContext *m_pGameServer;
	std::array<std::unique_ptr<IBlockClass>, static_cast<int>(EClassId::COUNT)> m_apClasses;
	std::array<int, MAX_CLIENTS> m_aPlayerClassByClient;

	int FindClassIndex(const char *pName) const;
	const IBlockClass *FindClassByQuery(const char *pQuery, int *pOutIndex) const;
	void AssignPlayerClass(int ClientId, int ClassIndex);
};

#endif
