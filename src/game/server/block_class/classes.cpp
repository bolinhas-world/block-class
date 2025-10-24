/* Block Class System - Class Manager Implementation */
#include "classes.h"
#include "classes/Soldier.h"
#include "classes/Ninja.h"

#include <game/server/gamecontext.h>
#include <game/server/player.h>
#include <game/server/entities/character.h>
#include <engine/shared/config.h>

CClassManager::CClassManager(CGameContext* pGameServer) :
	m_pGameServer(pGameServer)
{
	// Inicializar arrays
	mem_zero(m_aPlayerClasses, sizeof(m_aPlayerClasses));
	for(int i = 0; i < MAX_CLIENTS; i++)
	{
		m_aPlayerClassTypes[i] = CLASS_NONE;
	}
}

CClassManager::~CClassManager()
{
	Reset();
}

void CClassManager::Init()
{
	// Método de inicialização se necessário
}

void CClassManager::Reset()
{
	// Limpar todas as instâncias de classes
	for(int i = 0; i < MAX_CLIENTS; i++)
	{
		DestroyClassInstance(i);
	}
}

IPlayerClass* CClassManager::CreateClassInstance(EPlayerClass ClassType)
{
	switch(ClassType)
	{
		case CLASS_SOLDIER:
			return new CSoldierClass();
		case CLASS_NINJA:
			return new CNinjaClass();
		default:
			return nullptr;
	}
}

void CClassManager::DestroyClassInstance(int ClientId)
{
	if(!IsValidClientId(ClientId))
		return;

	if(m_aPlayerClasses[ClientId])
	{
		delete m_aPlayerClasses[ClientId];
		m_aPlayerClasses[ClientId] = nullptr;
	}
	
	m_aPlayerClassTypes[ClientId] = CLASS_NONE;
}

bool CClassManager::SetPlayerClass(int ClientId, EPlayerClass ClassType)
{
	if(!IsValidClientId(ClientId))
		return false;
		
	if(!IsValidClassType(ClassType))
		return false;

	// Destruir classe anterior se existir
	DestroyClassInstance(ClientId);

	// Criar nova instância
	IPlayerClass* pClass = CreateClassInstance(ClassType);
	if(!pClass)
		return false;

	m_aPlayerClasses[ClientId] = pClass;
	m_aPlayerClassTypes[ClientId] = ClassType;

	// Notificar jogador
	CPlayer* pPlayer = m_pGameServer->m_apPlayers[ClientId];
	if(pPlayer)
	{
		char aBuf[128];
		str_format(aBuf, sizeof(aBuf), "Class selected: %s", pClass->GetClassName());
		m_pGameServer->SendChatTarget(ClientId, aBuf);
		m_pGameServer->SendBroadcast(pClass->GetDescription(), ClientId);
	}

	return true;
}

EPlayerClass CClassManager::GetPlayerClass(int ClientId) const
{
	if(!IsValidClientId(ClientId))
		return CLASS_NONE;
		
	return m_aPlayerClassTypes[ClientId];
}

IPlayerClass* CClassManager::GetPlayerClassInstance(int ClientId) const
{
	if(!IsValidClientId(ClientId))
		return nullptr;
		
	return m_aPlayerClasses[ClientId];
}

const char* CClassManager::GetPlayerClassName(int ClientId) const
{
	IPlayerClass* pClass = GetPlayerClassInstance(ClientId);
	if(!pClass)
		return "None";
		
	return pClass->GetClassName();
}

void CClassManager::OnPlayerSpawn(CCharacter* pCharacter)
{
	if(!pCharacter)
		return;

	CPlayer* pPlayer = pCharacter->GetPlayer();
	if(!pPlayer)
		return;

	int ClientId = pPlayer->GetCid();
	IPlayerClass* pClass = GetPlayerClassInstance(ClientId);
	
	if(pClass)
	{
		pClass->OnSpawn(pCharacter);
	}
	else
	{
		// Jogador sem classe, mostrar menu de seleção
		OpenClassSelectionMenu(ClientId);
	}
}

void CClassManager::OnPlayerDeath(CCharacter* pCharacter, int Killer, int Weapon)
{
	if(!pCharacter)
		return;

	CPlayer* pPlayer = pCharacter->GetPlayer();
	if(!pPlayer)
		return;

	int ClientId = pPlayer->GetCid();
	IPlayerClass* pClass = GetPlayerClassInstance(ClientId);
	
	if(pClass)
	{
		pClass->OnDeath(pCharacter, Killer, Weapon);
	}
}

void CClassManager::OnPlayerTick(CCharacter* pCharacter)
{
	if(!pCharacter)
		return;

	CPlayer* pPlayer = pCharacter->GetPlayer();
	if(!pPlayer)
		return;

	int ClientId = pPlayer->GetCid();
	IPlayerClass* pClass = GetPlayerClassInstance(ClientId);
	
	if(pClass)
	{
		pClass->OnTick(pCharacter);
	}
}

void CClassManager::OnWeaponFire(CCharacter* pCharacter, int Weapon)
{
	if(!pCharacter)
		return;

	CPlayer* pPlayer = pCharacter->GetPlayer();
	if(!pPlayer)
		return;

	int ClientId = pPlayer->GetCid();
	IPlayerClass* pClass = GetPlayerClassInstance(ClientId);
	
	if(pClass)
	{
		pClass->OnWeaponFire(pCharacter, Weapon);
	}
}

void CClassManager::OnPlayerDamage(CCharacter* pCharacter, vec2 Force, int Dmg, int From, int Weapon)
{
	if(!pCharacter)
		return;

	CPlayer* pPlayer = pCharacter->GetPlayer();
	if(!pPlayer)
		return;

	int ClientId = pPlayer->GetCid();
	IPlayerClass* pClass = GetPlayerClassInstance(ClientId);
	
	if(pClass)
	{
		pClass->OnPlayerDamage(pCharacter, Force, Dmg, From, Weapon);
	}
}

void CClassManager::UsePlayerSpecialAbility(int ClientId)
{
	if(!IsValidClientId(ClientId))
		return;

	CPlayer* pPlayer = m_pGameServer->m_apPlayers[ClientId];
	if(!pPlayer)
		return;

	CCharacter* pCharacter = pPlayer->GetCharacter();
	if(!pCharacter)
		return;

	IPlayerClass* pClass = GetPlayerClassInstance(ClientId);
	if(pClass)
	{
		pClass->UseSpecialAbility(pCharacter);
	}
}

void CClassManager::OpenClassSelectionMenu(int ClientId)
{
	if(!IsValidClientId(ClientId))
		return;

	CPlayer* pPlayer = m_pGameServer->m_apPlayers[ClientId];
	if(!pPlayer)
		return;

	// Enviar informações sobre as classes disponíveis
	m_pGameServer->SendChatTarget(ClientId, "===== CLASS SELECTION =====");
	m_pGameServer->SendChatTarget(ClientId, "Type /class <name> to select a class:");
	m_pGameServer->SendChatTarget(ClientId, "");
	
	for(int i = 0; i < NUM_CLASSES; i++)
	{
		EPlayerClass ClassType = (EPlayerClass)i;
		const char* pName = GetClassNameByType(ClassType);
		const char* pDesc = GetClassDescriptionByType(ClassType);
		
		char aBuf[256];
		str_format(aBuf, sizeof(aBuf), "  %s - %s", pName, pDesc);
		m_pGameServer->SendChatTarget(ClientId, aBuf);
	}
	
	m_pGameServer->SendChatTarget(ClientId, "");
	m_pGameServer->SendChatTarget(ClientId, "Example: /class soldier");
}

void CClassManager::HandleClassSelection(int ClientId, int ClassIndex)
{
	if(!IsValidClientId(ClientId))
		return;

	EPlayerClass ClassType = (EPlayerClass)ClassIndex;
	
	if(SetPlayerClass(ClientId, ClassType))
	{
		// Respawn player com nova classe
		CPlayer* pPlayer = m_pGameServer->m_apPlayers[ClientId];
		if(pPlayer)
		{
			pPlayer->KillCharacter(WEAPON_GAME, false);
			pPlayer->Respawn();
		}
	}
}

const char* CClassManager::GetClassNameByType(EPlayerClass ClassType) const
{
	IPlayerClass* pTempClass = const_cast<CClassManager*>(this)->CreateClassInstance(ClassType);
	if(!pTempClass)
		return "Unknown";
		
	const char* pName = pTempClass->GetClassName();
	delete pTempClass;
	
	return pName;
}

const char* CClassManager::GetClassDescriptionByType(EPlayerClass ClassType) const
{
	IPlayerClass* pTempClass = const_cast<CClassManager*>(this)->CreateClassInstance(ClassType);
	if(!pTempClass)
		return "Unknown class";
		
	const char* pDesc = pTempClass->GetDescription();
	delete pTempClass;
	
	return pDesc;
}

bool CClassManager::IsValidClientId(int ClientId) const
{
	return ClientId >= 0 && ClientId < MAX_CLIENTS;
}

bool CClassManager::IsValidClassType(EPlayerClass ClassType) const
{
	return ClassType >= 0 && ClassType < NUM_CLASSES;
}
