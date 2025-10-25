#ifndef GAME_SERVER_BLOCK_CLASS_CLASS_MANAGER_H
#define GAME_SERVER_BLOCK_CLASS_CLASS_MANAGER_H

#include "block_class.h"
#include <array>
#include <memory>
#include <vector>

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
	void OnCharacterAddVelocity(CCharacter *pCharacter, vec2 &Addition);
	void OnCharacterTakeDamage(CCharacter *pCharacter, vec2 &Force, int &Damage, int From, int Weapon);
	void OnCharacterPostCoreTick(CCharacter *pCharacter);
	float AdjustWeaponFireDelay(int ClientId, CCharacter *pCharacter, int Weapon, float BaseDelay) const;
	bool RefreshClanForClient(int ClientId);
	bool RefreshClanForGroup(int ClientId);
	bool ApplyExistingClassForClient(int ClientId);
	bool HandleGunFire(int ClientId, CCharacter *pCharacter, vec2 ProjStartPos, vec2 Direction, vec2 MouseTarget);

	void ListClasses(int ClientId) const;
	bool HandleClassCommand(int ClientId, const char *pClassName);
	bool HandleHelpRequest(int ClientId, const char *pQuery) const;

private:
	enum class EClassId : int
	{
		Soldier = 0,
		Runner,
		Kangaro,
		Healer,
		Tank,
		COUNT
	};

	static constexpr int INVALID_CLASS = -1;

	CGameContext *m_pGameServer;
	std::array<std::unique_ptr<IBlockClass>, static_cast<int>(EClassId::COUNT)> m_apClasses;
	std::array<int, MAX_CLIENTS> m_aPlayerClassByClient;

	int FindClassIndex(const char *pName) const;
	const IBlockClass *FindClassByQuery(const char *pQuery, int *pOutIndex) const;
	std::vector<int> CollectLinkedClients(int ClientId) const;
	void AssignClassToSingleClient(int ClientId, int ClassIndex, bool IsRequestingClient);
	void AssignPlayerClass(int ClientId, int ClassIndex);
};

#endif
