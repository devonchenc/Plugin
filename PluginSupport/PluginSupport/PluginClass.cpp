
#include "stdafx.h"
#include "PluginClass.h"
#include "PluginLibrary.h"
#include "PluginSupport.h"
#include "PluginImpl.h"
#include "PluginWrapper.h"

int CPlugin::MergeMenu(const CMenu* pMenuAdd, BOOL bTopLevel)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CPluginSupportApp* pApp = (CPluginSupportApp*)AfxGetApp();
	CWinApp* pMainApp = pApp->GetMainApp();

	HMENU hMenu = NULL;
	pMainApp->m_pMainWnd->SendMessage(WM_MENU_EVENT, (WPARAM)FALSE, (LPARAM)&hMenu);
	ASSERT(hMenu != NULL);

	// calculate command offset
	CPluginWrapper* pPluginWrapper = pApp->GetPluginArray().GetAt(m_nPluginIndex);

	CMenu ParentMenu;
	ParentMenu.Attach(hMenu);
	int nCommandCount = MergeMenuImpl(&ParentMenu, pMenuAdd, pPluginWrapper, bTopLevel);
	// refresh menu
	pMainApp->m_pMainWnd->SendMessage(WM_MENU_EVENT, (WPARAM)TRUE, (LPARAM)&ParentMenu);
	ParentMenu.Detach();

	return nCommandCount;
}

// merge toolbar
int CPlugin::MergeToolbar(UINT nIDResource)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CPluginSupportApp* pApp = (CPluginSupportApp*)AfxGetApp();
	CWinApp* pMainApp = pApp->GetMainApp();

	CPluginWrapper* pPluginWrapper = pApp->GetPluginArray().GetAt(m_nPluginIndex);
	ToolbarInfo info;
	info.nPluginIndex = m_nPluginIndex;
	info.hInstance = pPluginWrapper->GetInstance();
	info.nIDResource = nIDResource;
	// calculate command offset
	info.nCommandIDIndex = CPluginWrapper::GetCommandIDIndex();
	int nCommandCount = pMainApp->m_pMainWnd->SendMessage(WM_TOOLBAR_EVENT, (WPARAM)TRUE, (LPARAM)&info);

	return nCommandCount;
}

// insert toolbar
int CPlugin::InsertToolbar(UINT nIDResource, CString strText)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CPluginSupportApp* pApp = (CPluginSupportApp*)AfxGetApp();
	CWinApp* pMainApp = pApp->GetMainApp();

	CPluginWrapper* pPluginWrapper = pApp->GetPluginArray().GetAt(m_nPluginIndex);
	ToolbarInfo info;
	info.nPluginIndex = m_nPluginIndex;
	info.hInstance = pPluginWrapper->GetInstance();
	info.nIDResource = nIDResource;
	// calculate command offset
	info.nCommandIDIndex = CPluginWrapper::GetCommandIDIndex();
	info.strText = strText;
	LRESULT nCommandCount = pMainApp->m_pMainWnd->SendMessage(WM_TOOLBAR_EVENT, (WPARAM)FALSE, (LPARAM)&info);

	return int(nCommandCount);
}

// this code was taken from the article
// http://www.codeproject.com/menu/mergemenu.asp
// by Oskar Wieland (http://www.codeproject.com/script/profile/whos_who.asp?id=23357)
int CPlugin::MergeMenuImpl(CMenu* pMenuDestination, const CMenu* pMenuAdd, CPluginWrapper* pPluginWrapper, BOOL bTopLevel /*=FALSE*/)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// Abstract:
	//		Merges two menus.
	//
	// Parameters:
	//		pMenuDestination	- [in, retval] destination menu handle
	//		pMenuAdd			- [in] menu to merge
	//		bTopLevel			- [in] indicator for special top level behavior
	//
	// Return value:
	//		<false> in case of error.
	//
	// Comments:
	//		This function calles itself recursivley. If bTopLevel is set to true,
	//		we append popups at top level or we insert before <Window> or <Help>.

	int nCommandCount = 0;
	// get the number menu items in the menus
	int iMenuAddItemCount = pMenuAdd->GetMenuItemCount();
	int iMenuDestItemCount = pMenuDestination->GetMenuItemCount();

	// if there are no items return
	if (iMenuAddItemCount == 0)
		return FALSE;

	// if we are not at top level and the destination menu is not empty
	// -> we append a seperator
	if (!bTopLevel && iMenuDestItemCount > 0)
	{
		pMenuDestination->AppendMenu(MF_SEPARATOR);
	}

	// iterate through the top level of 
	for (int iLoop = 0; iLoop < iMenuAddItemCount; iLoop++)
	{
		// get the menu string from the add menu
		CString sMenuAddString;
		pMenuAdd->GetMenuString(iLoop, sMenuAddString, MF_BYPOSITION);

		// try to get the submenu of the current menu item
		CMenu* pSubMenu = pMenuAdd->GetSubMenu(iLoop);

		// check if we have a sub menu
		if (!pSubMenu)
		{
			// normal menu item
			// read the source and append at the destination
			UINT nState = pMenuAdd->GetMenuState(iLoop, MF_BYPOSITION);
			UINT nItemID = pMenuAdd->GetMenuItemID(iLoop);

			if (pMenuDestination->AppendMenu(nState, CPluginWrapper::GetCommandIDIndex(), sMenuAddString))
			{
				// menu item added, don't forget to correct the item count
				iMenuDestItemCount++;
				nCommandCount++;

				// add new command
				pPluginWrapper->AddCommand(nItemID);
			}
			else
			{
				TRACE(_T("Plugin: AppendMenu failed!\n"));
				return FALSE;
			}
		}
		else
		{
			// create or insert a new popup menu item

			// default insert pos is like ap
			int iInsertPosDefault = -1;

			// if we are at top level merge into existing popups rather than
			// creating new ones
			if (bTopLevel)
			{
				ASSERT(sMenuAddString != "&?" && sMenuAddString != "?");
				CString sAdd(sMenuAddString);
				//	sAdd.Remove('&');  // for comparison of menu items supress '&'
				bool bAdded = false;

				// try to find existing popup
				for (int iLoop1 = 0; iLoop1 < iMenuDestItemCount; iLoop1++)
				{
					// get the menu string from the destination menu
					CString sDest;
					pMenuDestination->GetMenuString(iLoop1, sDest, MF_BYPOSITION);
					//	sDest.Remove('&'); // for a better compare (s.a.)

					if (sAdd == sDest)
					{
						// we got a hit -> merge the two popups
						// try to get the submenu of the desired destination menu item
						CMenu* pSubMenuDest = pMenuDestination->GetSubMenu(iLoop1);

						if (pSubMenuDest)
						{
							// merge the popup recursivly and continue with outer for loop
							int nSubCommandCount = MergeMenuImpl(pSubMenuDest, pSubMenu, pPluginWrapper, FALSE);
							if (!nSubCommandCount)
							{
								return FALSE;
							}
							else
							{
								nCommandCount += nSubCommandCount;
							}

							bAdded = true;
							break;
						}
					}

					// alternativ insert before <Window> or <Help>
					if (iInsertPosDefault == -1 && (sDest == "Window" || sDest == "?" || sDest == "Help"))
						iInsertPosDefault = iLoop1;
				}

				if (bAdded)
				{
					// menu added, so go on with loop over pMenuAdd's top level
					continue;
				}
			}

			// if the top level search did not find a position append the menu
			if (iInsertPosDefault == -1)
			{
				iInsertPosDefault = pMenuDestination->GetMenuItemCount();
			}

			// create a new popup and insert before <Window> or <Help>
			CMenu NewPopupMenu;
			if (!NewPopupMenu.CreatePopupMenu())
			{
				TRACE(_T("Plugin: CreatePopupMenu failed!\n"));
				return FALSE;
			}

			// merge the new popup recursivly
			int nSubCommandCount = MergeMenuImpl(&NewPopupMenu, pSubMenu, pPluginWrapper, FALSE);
			if (!nSubCommandCount)
			{
				return FALSE;
			}
			else
			{
				nCommandCount += nSubCommandCount;
			}

			// insert the new popup menu into the destination menu
			HMENU hNewMenu = NewPopupMenu.GetSafeHmenu();

			if (pMenuDestination->InsertMenu(-1, MF_BYPOSITION | MF_POPUP | MF_ENABLED, (UINT)hNewMenu, sMenuAddString))
			{
				// don't forget to correct the item count
				iMenuDestItemCount++;
			}
			else
			{
				TRACE(_T("Plugin: InsertMenu failed!\n"));
				return FALSE;
			}

			// don't destroy the new menu
			NewPopupMenu.Detach();
		}
	} 

	return nCommandCount;
}