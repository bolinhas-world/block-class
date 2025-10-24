/* Block Class System - Chat Commands */
#include "gamecontext.h"
#include "block_class/classes.h"
#include "block_class/classes/IPlayerClass.h"
#include "player.h"
#include "entities/character.h"

#include <engine/shared/config.h>

void CGameContext::ConSelectClass(IConsole::IResult *pResult, void *pUserData)
{
	CGameContext *pSelf = (CGameContext *)pUserData;
	if(!CheckClientId(pResult->m_ClientId))
		return;

	if(!pSelf->m_pClassManager)
	{
		pSelf->Console()->Print(IConsole::OUTPUT_LEVEL_STANDARD, "class", "Class system not initialized");
		return;
	}

	if(pResult->NumArguments() == 0)
	{
		// Show available classes
		pSelf->m_pClassManager->OpenClassSelectionMenu(pResult->m_ClientId);
		return;
	}

	const char *pClassName = pResult->GetString(0);
	
	// Map class name to enum
	EPlayerClass ClassType = CLASS_NONE;
	
	if(str_comp_nocase(pClassName, "soldier") == 0)
		ClassType = CLASS_SOLDIER;
	else if(str_comp_nocase(pClassName, "ninja") == 0)
		ClassType = CLASS_NINJA;
	else
	{
		char aBuf[128];
		str_format(aBuf, sizeof(aBuf), "Unknown class '%s'. Use /class to see available classes.", pClassName);
		pSelf->SendChatTarget(pResult->m_ClientId, aBuf);
		return;
	}

	// Set the class
	if(pSelf->m_pClassManager->SetPlayerClass(pResult->m_ClientId, ClassType))
	{
		// Respawn player with new class
		CPlayer *pPlayer = pSelf->m_apPlayers[pResult->m_ClientId];
		if(pPlayer)
		{
			pPlayer->KillCharacter(WEAPON_GAME, false);
			pPlayer->Respawn();
		}
	}
}

void CGameContext::ConClassInfo(IConsole::IResult *pResult, void *pUserData)
{
	CGameContext *pSelf = (CGameContext *)pUserData;
	if(!CheckClientId(pResult->m_ClientId))
		return;

	if(!pSelf->m_pClassManager)
	{
		pSelf->Console()->Print(IConsole::OUTPUT_LEVEL_STANDARD, "class", "Class system not initialized");
		return;
	}

	int ClientId = pResult->m_ClientId;
	
	pSelf->SendChatTarget(ClientId, "===== YOUR CLASS INFO =====");
	
	EPlayerClass CurrentClass = pSelf->m_pClassManager->GetPlayerClass(ClientId);
	
	if(CurrentClass == CLASS_NONE)
	{
		pSelf->SendChatTarget(ClientId, "You don't have a class selected.");
		pSelf->SendChatTarget(ClientId, "Type /class to select one.");
		return;
	}

	IPlayerClass *pClass = pSelf->m_pClassManager->GetPlayerClassInstance(ClientId);
	if(!pClass)
		return;

	char aBuf[256];
	
	str_format(aBuf, sizeof(aBuf), "Current Class: %s", pClass->GetClassName());
	pSelf->SendChatTarget(ClientId, aBuf);
	
	str_format(aBuf, sizeof(aBuf), "Description: %s", pClass->GetDescription());
	pSelf->SendChatTarget(ClientId, aBuf);
	
	pSelf->SendChatTarget(ClientId, "");
	pSelf->SendChatTarget(ClientId, "Stats:");
	
	str_format(aBuf, sizeof(aBuf), "  Max Health: %d", pClass->GetMaxHealth());
	pSelf->SendChatTarget(ClientId, aBuf);
	
	str_format(aBuf, sizeof(aBuf), "  Max Armor: %d", pClass->GetMaxArmor());
	pSelf->SendChatTarget(ClientId, aBuf);
	
	str_format(aBuf, sizeof(aBuf), "  Speed: %.0f%%", pClass->GetSpeedMultiplier() * 100.0f);
	pSelf->SendChatTarget(ClientId, aBuf);
	
	str_format(aBuf, sizeof(aBuf), "  Damage: %.0f%%", pClass->GetDamageMultiplier() * 100.0f);
	pSelf->SendChatTarget(ClientId, aBuf);
	
	if(pClass->GetHookLengthBonus() > 0)
	{
		str_format(aBuf, sizeof(aBuf), "  Hook Bonus: +%d tiles", pClass->GetHookLengthBonus());
		pSelf->SendChatTarget(ClientId, aBuf);
	}
	
	pSelf->SendChatTarget(ClientId, "");
	
	if(pClass->GetSpecialCooldown() > 0)
	{
		int CooldownSeconds = pClass->GetSpecialCooldown() / 50;
		str_format(aBuf, sizeof(aBuf), "Special Ability: Use /special (Cooldown: %ds)", CooldownSeconds);
		pSelf->SendChatTarget(ClientId, aBuf);
	}
}

void CGameContext::ConUseSpecial(IConsole::IResult *pResult, void *pUserData)
{
	CGameContext *pSelf = (CGameContext *)pUserData;
	if(!CheckClientId(pResult->m_ClientId))
		return;

	if(!pSelf->m_pClassManager)
	{
		pSelf->Console()->Print(IConsole::OUTPUT_LEVEL_STANDARD, "class", "Class system not initialized");
		return;
	}

	int ClientId = pResult->m_ClientId;
	CPlayer *pPlayer = pSelf->m_apPlayers[ClientId];
	
	if(!pPlayer || !pPlayer->GetCharacter())
	{
		pSelf->SendChatTarget(ClientId, "You must be alive to use your special ability!");
		return;
	}

	EPlayerClass CurrentClass = pSelf->m_pClassManager->GetPlayerClass(ClientId);
	
	if(CurrentClass == CLASS_NONE)
	{
		pSelf->SendChatTarget(ClientId, "You don't have a class selected. Type /class to select one.");
		return;
	}

	// Use the special ability
	pSelf->m_pClassManager->UsePlayerSpecialAbility(ClientId);
}
