#include "class_manager.h"

#include "healer.h"
#include "runner.h"
#include "soldier.h"
#include "tank.h"

#include <base/system.h>

#include <engine/server.h>
#include <engine/shared/uuid_manager.h>

#include <game/server/entities/character.h>
#include <game/server/gamecontext.h>
#include <game/server/player.h>

CBlockClassManager::CBlockClassManager(CGameContext *pGameServer) :
	m_pGameServer(pGameServer)
{
	m_apClasses[static_cast<int>(EClassId::Soldier)] = std::make_unique<CBlockClassSoldier>();
	m_apClasses[static_cast<int>(EClassId::Runner)] = std::make_unique<CBlockClassRunner>();
	m_apClasses[static_cast<int>(EClassId::Healer)] = std::make_unique<CBlockClassHealer>();
	m_apClasses[static_cast<int>(EClassId::Tank)] = std::make_unique<CBlockClassTank>();
	m_aPlayerClassByClient.fill(INVALID_CLASS);
}

void CBlockClassManager::ResetPlayer(int ClientId)
{
	if(ClientId < 0 || ClientId >= MAX_CLIENTS)
	{
		return;
	}

	m_aPlayerClassByClient[ClientId] = INVALID_CLASS;
	RefreshClanForClient(ClientId);
}

void CBlockClassManager::HandlePlayerDisconnect(int ClientId)
{
	if(ClientId < 0 || ClientId >= MAX_CLIENTS)
	{
		return;
	}

	m_pGameServer->SendChatTarget(ClientId, "Classes disponiveis:");

	const int Current = m_aPlayerClassByClient[ClientId];
	if(Current >= 0)
	{
		CPlayer *pPlayer = m_pGameServer->m_apPlayers[ClientId];
		if(pPlayer)
		{
			m_apClasses[Current]->OnUnassigned(m_pGameServer, pPlayer);
		}
	}

	m_aPlayerClassByClient[ClientId] = INVALID_CLASS;
	RefreshClanForClient(ClientId);
}

void CBlockClassManager::OnCharacterSpawn(CCharacter *pCharacter)
{
	if(!pCharacter)
	{
		return;
	}

	CPlayer *pPlayer = pCharacter->GetPlayer();
	if(!pPlayer)
	{
		return;
	}

	const int ClientId = pPlayer->GetCid();
	if(ClientId < 0 || ClientId >= MAX_CLIENTS)
	{
		return;
	}

	const int ClassIndex = m_aPlayerClassByClient[ClientId];
	if(ClassIndex >= 0)
	{
		m_apClasses[ClassIndex]->OnCharacterSpawn(m_pGameServer, pCharacter);
	}
}

void CBlockClassManager::OnCharacterTick(CCharacter *pCharacter)
{
	if(!pCharacter)
	{
		return;
	}

	CPlayer *pPlayer = pCharacter->GetPlayer();
	if(!pPlayer)
	{
		return;
	}

	const int ClientId = pPlayer->GetCid();
	if(ClientId < 0 || ClientId >= MAX_CLIENTS)
	{
		return;
	}

	const int ClassIndex = m_aPlayerClassByClient[ClientId];
	if(ClassIndex >= 0)
	{
		m_apClasses[ClassIndex]->OnCharacterTick(m_pGameServer, pCharacter);
	}
}

void CBlockClassManager::OnExplosionHit(int OwnerId, int Weapon, CCharacter *pOwner, CCharacter *pTarget, vec2 HitPos, vec2 ForceDir)
{
	if(OwnerId < 0 || OwnerId >= MAX_CLIENTS)
	{
		return;
	}

	const int ClassIndex = m_aPlayerClassByClient[OwnerId];
	if(ClassIndex >= 0)
	{
		m_apClasses[ClassIndex]->OnExplosionHit(m_pGameServer, Weapon, pOwner, pTarget, HitPos, ForceDir);
	}
}

void CBlockClassManager::OnProjectileHit(int OwnerId, int Weapon, CCharacter *pOwner, CCharacter *pTarget, vec2 HitPos)
{
	if(OwnerId < 0 || OwnerId >= MAX_CLIENTS)
	{
		return;
	}

	const int ClassIndex = m_aPlayerClassByClient[OwnerId];
	if(ClassIndex >= 0)
	{
		m_apClasses[ClassIndex]->OnProjectileHit(m_pGameServer, Weapon, pOwner, pTarget, HitPos);
	}
}

void CBlockClassManager::OnCharacterAddVelocity(CCharacter *pCharacter, vec2 &Addition)
{
	if(!pCharacter)
	{
		return;
	}

	CPlayer *pPlayer = pCharacter->GetPlayer();
	if(!pPlayer)
	{
		return;
	}

	const int ClientId = pPlayer->GetCid();
	if(ClientId < 0 || ClientId >= MAX_CLIENTS)
	{
		return;
	}

	const int ClassIndex = m_aPlayerClassByClient[ClientId];
	if(ClassIndex >= 0)
	{
		m_apClasses[ClassIndex]->OnCharacterAddVelocity(m_pGameServer, pCharacter, Addition);
	}
}

void CBlockClassManager::OnCharacterTakeDamage(CCharacter *pCharacter, vec2 &Force, int &Damage, int From, int Weapon)
{
	if(!pCharacter)
	{
		return;
	}

	CPlayer *pPlayer = pCharacter->GetPlayer();
	if(!pPlayer)
	{
		return;
	}

	const int ClientId = pPlayer->GetCid();
	if(ClientId < 0 || ClientId >= MAX_CLIENTS)
	{
		return;
	}

	const int ClassIndex = m_aPlayerClassByClient[ClientId];
	if(ClassIndex >= 0)
	{
		m_apClasses[ClassIndex]->OnCharacterTakeDamage(m_pGameServer, pCharacter, Force, Damage, From, Weapon);
	}
}

void CBlockClassManager::OnCharacterPostCoreTick(CCharacter *pCharacter)
{
	if(!pCharacter)
	{
		return;
	}

	CPlayer *pPlayer = pCharacter->GetPlayer();
	if(!pPlayer)
	{
		return;
	}

	const int ClientId = pPlayer->GetCid();
	if(ClientId < 0 || ClientId >= MAX_CLIENTS)
	{
		return;
	}

	const int ClassIndex = m_aPlayerClassByClient[ClientId];
	if(ClassIndex >= 0)
	{
		m_apClasses[ClassIndex]->OnCharacterPostCoreTick(m_pGameServer, pCharacter);
	}
}

float CBlockClassManager::AdjustWeaponFireDelay(int ClientId, CCharacter *pCharacter, int Weapon, float BaseDelay) const
{
	if(ClientId < 0 || ClientId >= MAX_CLIENTS)
	{
		return BaseDelay;
	}

	const int ClassIndex = m_aPlayerClassByClient[ClientId];
	if(ClassIndex < 0)
	{
		return BaseDelay;
	}

	const IBlockClass *pClass = m_apClasses[ClassIndex].get();
	if(!pClass)
	{
		return BaseDelay;
	}

	return pClass->AdjustWeaponFireDelay(m_pGameServer, pCharacter, Weapon, BaseDelay);
}

void CBlockClassManager::ListClasses(int ClientId) const
{
	if(ClientId < 0 || ClientId >= MAX_CLIENTS)
	{
		return;
	}

	const int Current = m_aPlayerClassByClient[ClientId];
	for(int Index = 0; Index < static_cast<int>(EClassId::COUNT); ++Index)
	{
		const IBlockClass *pClass = m_apClasses[Index].get();
		if(!pClass)
		{
			continue;
		}

		char aBuf[192];
		str_format(aBuf, sizeof(aBuf), "%s: %s", pClass->DisplayName(), pClass->Description());
		m_pGameServer->SendChatTarget(ClientId, aBuf);

		if(Index == Current)
		{
			str_format(aBuf, sizeof(aBuf), "Voce ja esta usando /class %s.", pClass->CommandName());
		}
		else
		{
			str_format(aBuf, sizeof(aBuf), "Use /class %s para selecionar.", pClass->CommandName());
		}
		m_pGameServer->SendChatTarget(ClientId, aBuf);
	}
}

bool CBlockClassManager::HandleClassCommand(int ClientId, const char *pClassName)
{
	if(ClientId < 0 || ClientId >= MAX_CLIENTS || !pClassName)
	{
		return false;
	}

	const int ClassIndex = FindClassIndex(pClassName);
	if(ClassIndex == INVALID_CLASS)
	{
		return false;
	}

	AssignPlayerClass(ClientId, ClassIndex);
	return true;
}

bool CBlockClassManager::HandleHelpRequest(int ClientId, const char *pQuery) const
{
	if(ClientId < 0 || ClientId >= MAX_CLIENTS || !pQuery)
	{
		return false;
	}

	int ClassIndex = INVALID_CLASS;
	const IBlockClass *pClass = FindClassByQuery(pQuery, &ClassIndex);
	if(!pClass)
	{
		return false;
	}

	char aBuf[192];
	str_format(aBuf, sizeof(aBuf), "%s: %s", pClass->DisplayName(), pClass->Description());
	m_pGameServer->SendChatTarget(ClientId, aBuf);
	str_format(aBuf, sizeof(aBuf), "Use /class %s para selecionar.", pClass->CommandName());
	m_pGameServer->SendChatTarget(ClientId, aBuf);
	return true;
}

int CBlockClassManager::FindClassIndex(const char *pName) const
{
	if(!pName)
	{
		return INVALID_CLASS;
	}

	for(int Index = 0; Index < static_cast<int>(EClassId::COUNT); ++Index)
	{
		const IBlockClass *pClass = m_apClasses[Index].get();
		if(!pClass)
		{
			continue;
		}

		if(str_comp_nocase(pClass->CommandName(), pName) == 0 || str_comp_nocase(pClass->DisplayName(), pName) == 0)
		{
			return Index;
		}
	}

	return INVALID_CLASS;
}

const IBlockClass *CBlockClassManager::FindClassByQuery(const char *pQuery, int *pOutIndex) const
{
	if(pOutIndex)
	{
		*pOutIndex = INVALID_CLASS;
	}

	const int Index = FindClassIndex(pQuery);
	if(Index == INVALID_CLASS)
	{
		return nullptr;
	}

	if(pOutIndex)
	{
		*pOutIndex = Index;
	}

	return m_apClasses[Index].get();
}

std::vector<int> CBlockClassManager::CollectLinkedClients(int ClientId) const
{
	std::vector<int> Clients;
	if(ClientId < 0 || ClientId >= MAX_CLIENTS)
	{
		return Clients;
	}

	Clients.push_back(ClientId);

	if(!m_pGameServer)
	{
		return Clients;
	}

	IServer *pServer = m_pGameServer->Server();
	if(!pServer)
	{
		return Clients;
	}

	IServer::CClientInfo Info{};
	if(!pServer->GetClientInfo(ClientId, &Info) || !Info.m_pConnectionId)
	{
		return Clients;
	}

	const CUuid ConnectionId = *Info.m_pConnectionId;
	for(int OtherId = 0; OtherId < MAX_CLIENTS; ++OtherId)
	{
		if(OtherId == ClientId)
		{
			continue;
		}

		if(!m_pGameServer->m_apPlayers[OtherId])
		{
			continue;
		}

		IServer::CClientInfo OtherInfo{};
		if(!pServer->GetClientInfo(OtherId, &OtherInfo) || !OtherInfo.m_pConnectionId)
		{
			continue;
		}

		if(*OtherInfo.m_pConnectionId == ConnectionId)
		{
			Clients.push_back(OtherId);
		}
	}

	return Clients;
}

bool CBlockClassManager::RefreshClanForClient(int ClientId)
{
	if(ClientId < 0 || ClientId >= MAX_CLIENTS || !m_pGameServer)
	{
		return false;
	}

	if(!m_pGameServer->m_apPlayers[ClientId])
	{
		return false;
	}

	IServer *pServer = m_pGameServer->Server();
	if(!pServer || pServer->ClientSlotEmpty(ClientId))
	{
		return false;
	}

	const int ClassIndex = m_aPlayerClassByClient[ClientId];
	const char *pDesiredClan = "";
	if(ClassIndex >= 0)
	{
		const IBlockClass *pClass = m_apClasses[ClassIndex].get();
		if(pClass)
		{
			pDesiredClan = pClass->DisplayName();
		}
	}

	const char *pCurrentClan = pServer->ClientClan(ClientId);
	if(pCurrentClan && str_comp(pCurrentClan, pDesiredClan) == 0)
	{
		return false;
	}

	pServer->SetClientClan(ClientId, pDesiredClan);
	return true;
}

bool CBlockClassManager::RefreshClanForGroup(int ClientId)
{
	bool Changed = false;
	for(int LinkedId : CollectLinkedClients(ClientId))
	{
		Changed |= RefreshClanForClient(LinkedId);
	}
	return Changed;
}

bool CBlockClassManager::ApplyExistingClassForClient(int ClientId)
{
	if(ClientId < 0 || ClientId >= MAX_CLIENTS)
	{
		return false;
	}

	const std::vector<int> Clients = CollectLinkedClients(ClientId);
	int SourceClass = INVALID_CLASS;
	for(int LinkedId : Clients)
	{
		if(LinkedId == ClientId)
		{
			continue;
		}

		const int ClassIndex = m_aPlayerClassByClient[LinkedId];
		if(ClassIndex >= 0)
		{
			SourceClass = ClassIndex;
			break;
		}
	}

	if(SourceClass == INVALID_CLASS)
	{
		return false;
	}

	AssignClassToSingleClient(ClientId, SourceClass, false);
	return true;
}

void CBlockClassManager::AssignClassToSingleClient(int ClientId, int ClassIndex, bool IsRequestingClient)
{
	if(ClientId < 0 || ClientId >= MAX_CLIENTS || ClassIndex < 0 || ClassIndex >= static_cast<int>(EClassId::COUNT))
	{
		return;
	}

	CPlayer *pPlayer = m_pGameServer->m_apPlayers[ClientId];
	if(!pPlayer)
	{
		return;
	}

	const int Current = m_aPlayerClassByClient[ClientId];
	if(Current == ClassIndex)
	{
		if(IsRequestingClient)
		{
			char aBuf[128];
			str_format(aBuf, sizeof(aBuf), "Voce ja esta com a classe %s.", m_apClasses[ClassIndex]->DisplayName());
			m_pGameServer->SendChatTarget(ClientId, aBuf);
		}
		RefreshClanForClient(ClientId);
		return;
	}

	if(Current >= 0)
	{
		m_apClasses[Current]->OnUnassigned(m_pGameServer, pPlayer);
	}

	m_aPlayerClassByClient[ClientId] = ClassIndex;
	IBlockClass *pClass = m_apClasses[ClassIndex].get();
	pClass->OnAssigned(m_pGameServer, pPlayer);

	if(CCharacter *pChar = pPlayer->GetCharacter())
	{
		pClass->OnCharacterSpawn(m_pGameServer, pChar);
	}

	char aBuf[128];
	str_format(aBuf, sizeof(aBuf), "Classe %s escolhida.", pClass->DisplayName());
	m_pGameServer->SendChatTarget(ClientId, aBuf);
	RefreshClanForClient(ClientId);
}

void CBlockClassManager::AssignPlayerClass(int ClientId, int ClassIndex)
{
	if(ClientId < 0 || ClientId >= MAX_CLIENTS || ClassIndex < 0 || ClassIndex >= static_cast<int>(EClassId::COUNT))
	{
		return;
	}

	const std::vector<int> Clients = CollectLinkedClients(ClientId);
	for(int LinkedClientId : Clients)
	{
		AssignClassToSingleClient(LinkedClientId, ClassIndex, LinkedClientId == ClientId);
	}
}
