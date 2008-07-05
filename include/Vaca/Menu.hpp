// Vaca - Visual Application Components Abstraction
// Copyright (c) 2005, 2006, 2007, David A. Capello
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
//
// * Redistributions of source code must retain the above copyright
//   notice, this list of conditions and the following disclaimer.
// * Redistributions in binary form must reproduce the above copyright
//   notice, this list of conditions and the following disclaimer in
//   the documentation and/or other materials provided with the
//   distribution.
// * Neither the name of the Vaca nor the names of its contributors
//   may be used to endorse or promote products derived from this
//   software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
// FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
// COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
// INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
// HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
// STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
// OF THE POSSIBILITY OF SUCH DAMAGE.

#ifndef VACA_MENU_HPP
#define VACA_MENU_HPP

/**
 * First automatic ID generated for a MenuItem when its ID isn't
 * specified.
 */
#define VACA_FIRST_AUTOID 1000

#ifndef MIM_MENUDATA
#  define MIM_MENUDATA 8
#endif

#ifndef MIM_STYLE
#  define MIM_STYLE 16
#endif

#include <vector>
#include <boost/signal.hpp>
#include <boost/noncopyable.hpp>

#include "Vaca/base.hpp"
#include "Vaca/Component.hpp"
#include "Vaca/String.hpp"
#include "Vaca/Event.hpp"
#include "Vaca/Keys.hpp"

namespace Vaca {

class MenuItemEvent;
class MenuItem;
class Menu;
class MenuBar;
class MdiListMenu;

/**
 * A menu item.
 */
class VACA_DLL MenuItem : private boost::noncopyable
			, public Component
{
  friend class Menu;

  Menu  *m_parent;
  String m_text;
  int    m_id;
  std::vector<Keys::Type> m_shortcuts;

public:

  MenuItem();
  MenuItem(const String& text, Keys::Type defaultShortcut = Keys::None, int id = -1);
  virtual ~MenuItem();

  Menu *getParent();
  int getId();

  const String& getText();
  void setText(const String& text);

  bool isEnabled();
  void setEnabled(bool state);

  bool isChecked();
  void setChecked(bool state);
  void setRadio(bool state);

  void addShortcut(Keys::Type shortcut);
//   void addShortcut(Shortcut shortcut);

  virtual MenuItem *checkShortcuts(Keys::Type pressedKey);

  virtual bool isMenu() const;
  virtual bool isSeparator() const;
  virtual bool isMdiList() const;

  // events
  virtual void onAction(MenuItemEvent& ev);
  virtual void onUpdate(MenuItemEvent& ev);

  // signals
  boost::signal<void (MenuItemEvent&)> Action; ///< @see onAction
  boost::signal<void (MenuItemEvent&)> Update; ///< @see onUpdate
};

/**
 * A separator for menu items.
 */
class VACA_DLL MenuSeparator : public MenuItem
{
public:
  
  MenuSeparator();
  virtual ~MenuSeparator();

  virtual bool isSeparator() const;
  
};

//   class CheckBoxMenuItem : public MenuItem {
//   public:
//     CheckBoxMenuItem(const String& text, bool state = false);
//     virtual ~CheckBoxMenuItem();

//     bool getState();
//     void setState(bool state);
//   };

//   class RadioButtonMenuItem : public MenuItem {
//   public:
//     RadioButtonMenuItem(const String& text, bool selected = false);
//     virtual ~RadioButtonMenuItem();
//   };

/**
 * A container of menu items.
 */
class VACA_DLL Menu : public MenuItem
{
public:

  typedef std::vector<MenuItem*> Container;

private:

  HMENU m_HMENU;
  Container m_container;

public:

  Menu();
  explicit Menu(const String& text);
  explicit Menu(int menuId);
  explicit Menu(HMENU hmenu);
  virtual ~Menu();

  MenuItem* add(MenuItem* menuItem);
  MenuItem* add(const String& text, Keys::Type defaultShortcut = Keys::None);
  void addSeparator();

  MenuItem* insert(int index, MenuItem* menuItem);
  MenuItem* insert(int index, const String& text);
  void insertSeparator(int index);

  MenuItem* remove(MenuItem* menuItem);
  MenuItem* remove(int index);

  MenuItem* getMenuItemByIndex(int index);
  MenuItem* getMenuItemById(int id);
  int getMenuItemIndex(MenuItem* menuItem);
  // int getFirstMenuItemIndexByRadio(MenuItem* menuItem);
  // int getLastMenuItemIndexByRadio(MenuItem* menuItem);
  int getItemCount();
  Container getMenuItems();

  virtual MenuItem* checkShortcuts(Keys::Type pressedKey);

  // PopupMenu* getPopupMenu();

  virtual bool isMenu() const;

//   Menu* getMenuByHMENU(HMENU hmenu);

  HMENU getHMENU();

private:

  void subClass();
  
};

/**
 * A menu bar, has a collection of Menus.
 */
class VACA_DLL MenuBar : public Menu
{
public:

  MenuBar();
  explicit MenuBar(int menuId);
  virtual ~MenuBar();

  MdiListMenu* getMdiListMenu();

};

/** 
 * A menu that has the list of all MdiChild.
 */
class VACA_DLL MdiListMenu : public Menu
{
public:

  MdiListMenu(const String& text);
  virtual ~MdiListMenu();

  virtual bool isMdiList() const;

};

/*
// TODO PopupMenu
class VACA_DLL PopupMenu
{
public:

  PopupMenu();
  virtual ~PopupMenu();

  MenuItem* add(MenuItem* menuitem);
  MenuItem* add(const String& text);
  MenuItem* insert(MenuItem* menuitem, int index);
  MenuItem* insert(const String& text, int index);
  void remove(MenuItem* menuitem);
  void remove(int index);

};
*/
} // namespace Vaca

#endif
