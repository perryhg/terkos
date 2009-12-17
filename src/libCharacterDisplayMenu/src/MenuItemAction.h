//
// This file is part of Terk and TerkOS.
//
// All Terk and TerkOS source code is provided under the terms of the
// GNU General Public License v2 (http://www.gnu.org/licenses/gpl-2.0.html).
// Those wishing to use Terk and TerkOS source code, software and/or
// technologies under different licensing terms should contact us at
// telepresencerobotkit@cs.cmu.edu. Such licensing terms are available for
// all portions of the Terk and TerkOS codebase presented here.
//

/*
 * MenuItemAction is a pure abstract base class, defining an interface for menu item actions.
 *
 * Author: Chris Bartley (bartley@cmu.edu)
 */

#ifndef MENUITEMACTION_H_
#define MENUITEMACTION_H_

class MenuItemAction
   {
   public:

      virtual ~MenuItemAction()
         {
         }

      virtual void activate() = 0;
      virtual void deactivate() = 0;
      virtual void start() = 0;
      virtual void stop() = 0;
      virtual void upEvent() = 0;
      virtual void rightEvent() = 0;
      virtual void downEvent() = 0;
      virtual void leftEvent() = 0;
   };

#endif /* MENUITEMACTION_H_ */
