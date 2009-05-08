/*
 * Chris Bartley (bartley@cmu.edu)
 */

#include "CharacterDisplayMenu.h"

const string CharacterDisplayMenu::EMPTY_STRING = "";
const unsigned int CharacterDisplayMenu::DYNAMIC_OBJECT_ARGUMENT_ARRAY_SIZE = 4;

CharacterDisplayMenu::CharacterDisplayMenu(const string& xmlFilename, MenuStatusManager* menuStatusManager, CharacterDisplay* characterDisplay)
   {
   menuItem = NULL;

   // Turn off condensing of white space
   TiXmlBase::SetCondenseWhiteSpace(false);

   // First find the root element in the XML document
   ticpp::Element* rootElement = NULL;

   try
      {
      // Load a document
      ticpp::Document doc(xmlFilename);
      doc.LoadFile();

      std::cout << "CharacterDisplayMenu: Menu XML file loaded" << std::endl;

      // look for the <menu> element (FirstChild() might return the xml or doctype element)
      rootElement = doc.FirstChildElement(false);

      if (!rootElement || (rootElement->Value() != "menu") || rootElement->NoChildren())
         {
         std::cout << "ERROR: Invalid XML: The <menu> element is missing or contains no child elements!" << std::endl;
         rootElement = NULL;
         }

      std::cout << "CharacterDisplayMenu: building menu tree" << std::endl;

      // now build the tree
      if (rootElement != NULL)
         {
         menuItem = this->buildMenuItemTree(rootElement, menuStatusManager, characterDisplay);
         }
      }
   catch (ticpp::Exception& ex)
      {
      std::cout << "Exception while procesing the menu XML!" << std::endl;
      std::cout << ex.what();
      }
   }

MenuItemImpl* CharacterDisplayMenu::buildMenuItemTree(ticpp::Element* parentElement, MenuStatusManager* menuStatusManager,
                                                      CharacterDisplay* characterDisplay)
   {
   if (parentElement != NULL)
      {
      std::cout << "CharacterDisplayMenu::buildMenuItemTree: processing element <" << parentElement->Value() << ">" << std::endl;

      string text = parentElement->GetAttribute("text");

      bool hasItemChildElements = (NULL != parentElement->FirstChildElement("item", false));

      if (hasItemChildElements)
         {
         std::cout << "CharacterDisplayMenu::buildMenuItemTree: parent has <item> children" << std::endl;

         // recursively build the children
         vector<MenuItemImpl*> children;
         ticpp::Element* itemElement = NULL;
         for (itemElement = parentElement->FirstChildElement("item", false); itemElement; itemElement
                  = itemElement->NextSiblingElement("item", false))
            {

            children.push_back(this->buildMenuItemTree(itemElement, menuStatusManager, characterDisplay));
            }

         // create the MenuItem
         MenuItemImpl* menuItem = new MenuItemImpl(text, children);

         // set the MenuItemAction
         menuItem->setMenuItemAction(new CharacterDisplayMenuItemAction(NULL, menuItem, menuStatusManager, characterDisplay));

         // set up the parent and sibling references for each of the children
         unsigned int lastIndex = children.size() - 1;
         for (unsigned int menuIndex = 0; menuIndex < children.size(); menuIndex++)
            {
            MenuItem* previousSibling = children[menuIndex > 0 ? menuIndex - 1 : lastIndex];
            MenuItem* nextSibling = children[menuIndex < lastIndex ? menuIndex + 1 : 0];
            MenuItemImpl* child = children[menuIndex];
            child->setParent(menuItem);
            child->setPreviousSibling(previousSibling);
            child->setNextSibling(nextSibling);
            }

         return menuItem;
         }
      else
         {
         std::cout << "CharacterDisplayMenu::buildMenuItemTree: parent has NO <item> children" << std::endl;

         // create the MenuItem
         MenuItemImpl* menuItem = new MenuItemImpl(text);

         // create an instance of the MenuItemAction implementation class, or default
         // to the CharacterDisplayMenuItemAction upon failure
         MenuItemAction* menuItemAction = NULL;
         ticpp::Element* implementationClassElement = parentElement->FirstChildElement("implementation-class", false);
         if (implementationClassElement != NULL)
            {
            // See if the <implementation-class> element has any <property> child
            // elements.  If so, store them in a map of strings.
            map<string, string> properties;
            ticpp::Element* propertyElement = NULL;
            for (propertyElement = implementationClassElement->FirstChildElement("property", false); propertyElement; propertyElement
                     = propertyElement->NextSiblingElement("property", false))
               {
               string key = propertyElement->GetAttribute("key");

               // Use the element's text contents if the attribute isn't specified.
               string value;
               if (propertyElement->HasAttribute("value"))
                  {
                  value = propertyElement->GetAttribute("value");
                  }
               else
                  {
                  value = propertyElement->GetText(false);
                  }

               properties[key] = value;
               }

            // fetch the implementation class name
            string menuItemActionImplementationClassName = implementationClassElement->GetAttribute("name");

            // instantiate the class
            if (menuItemActionImplementationClassName != CharacterDisplayMenu::EMPTY_STRING)
               {
               cout << "impl class='" << menuItemActionImplementationClassName << "'" << endl;

               menuItemAction
                        = this->instantiateMenuItemAction("lib" + menuItemActionImplementationClassName + ".so", menuItemActionImplementationClassName, menuItem, menuStatusManager, characterDisplay, properties);
               }
            }

         if (menuItemAction == NULL)
            {
            cout << "CharacterDisplayMenu::buildMenuItemTree(): MenuItemAction implementation "
                     << "unspecified or invalid, defaulting to using CharacterDisplayMenuItemAction" << endl;
            menuItemAction = new CharacterDisplayMenuItemAction(NULL, menuItem, menuStatusManager, characterDisplay);
            }
         menuItem->setMenuItemAction(menuItemAction);

         return menuItem;
         }
      }

   return NULL;
   }

MenuItemAction* CharacterDisplayMenu::instantiateMenuItemAction(const string& sharedLibraryName, const string& className, MenuItem* menuItem,
                                                                MenuStatusManager* menuStatusManager, CharacterDisplay* characterDisplay, map<string,
                                                                         string>& properties)
   {
   DynamicLibrary* dynamicLibrary = DynamicLoader::loadObjectFile(sharedLibraryName.c_str(), RTLD_NOW);

   if (dynamicLibrary == NULL)
      {
      cerr << "CharacterDisplayMenu::instantiateMenuItemAction: Couldn't load dynamic library [" << sharedLibraryName << "]" << endl;
      }
   else
      {
      cout << "CharacterDisplayMenu::instantiateMenuItemAction: Loaded dynamic library [" << sharedLibraryName << "]" << endl;

      // keep track of the created dynamic libraries so we can delete them in the destructor
      dynamicLibraries.push_back(dynamicLibrary);

      // create the argument array
      void* args[CharacterDisplayMenu::DYNAMIC_OBJECT_ARGUMENT_ARRAY_SIZE];
      args[0] = menuItem;
      args[1] = menuStatusManager;
      args[2] = characterDisplay;
      args[3] = &properties;

      // create the MenuItemAction
      MenuItemAction
               * menuItemAction =
                        dynamic_cast<MenuItemAction*> (dynamicLibrary->newObject(className.c_str(), CharacterDisplayMenu::DYNAMIC_OBJECT_ARGUMENT_ARRAY_SIZE, args));

      if (menuItemAction == NULL)
         {
         cerr << "Couldn't create MenuItemAction [" << className << "]" << endl;
         }

      return menuItemAction;
      }

   return NULL;
   }

