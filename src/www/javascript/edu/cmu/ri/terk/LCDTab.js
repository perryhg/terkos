//======================================================================================================================
// This file is part of Terk and TerkOS.
//
// All Terk and TerkOS source code is provided under the terms of the GNU General Public License v2
// (http://www.gnu.org/licenses/gpl-2.0.html). Those wishing to use Terk and TerkOS source code, software and/or
// technologies under different licensing terms should contact us at telepresencerobotkit@cs.cmu.edu. Such licensing
// terms are available for all portions of the Terk and TerkOS codebase presented here.
//======================================================================================================================
// Class for initializing and configuring the various widgets on the LCD tab.
//
// Dependencies:
// * jQuery (http://jquery.com/)
// * jQuery UI (http://jqueryui.com/)
// * jquery.mousecapture.js (http://benanne.net/code/?p=238)
// * edu.cmu.ri.terk.NonClosableWaitDialog
// * edu.cmu.ri.terk.SingleButtonModalDialog
// * main.css
//
// Author: Chris Bartley (bartley@cmu.edu)
//======================================================================================================================

//======================================================================================================================
// VERIFY NAMESPACE
//======================================================================================================================
// Create the global symbol "edu" if it doesn't exist.  Throw an error if it does exist but is not an object.
var edu;
if (!edu)
   {
   edu = {};
   }
else
   {
   if (typeof edu != "object")
      {
      var eduExistsMessage = "Error: failed to create edu namespace: edu already exists and is not an object";
      alert(eduExistsMessage);
      throw new Error(eduExistsMessage);
      }
   }

// Repeat the creation and type-checking for the next level
if (!edu.cmu)
   {
   edu.cmu = {};
   }
else
   {
   if (typeof edu.cmu != "object")
      {
      var eduCmuExistsMessage = "Error: failed to create edu.cmu namespace: edu.cmu already exists and is not an object";
      alert(eduCmuExistsMessage);
      throw new Error(eduCmuExistsMessage);
      }
   }

// Repeat the creation and type-checking for the next level
if (!edu.cmu.ri)
   {
   edu.cmu.ri = {};
   }
else
   {
   if (typeof edu.cmu.ri != "object")
      {
      var eduCmuRiExistsMessage = "Error: failed to create edu.cmu.ri namespace: edu.cmu.ri already exists and is not an object";
      alert(eduCmuRiExistsMessage);
      throw new Error(eduCmuRiExistsMessage);
      }
   }

// Repeat the creation and type-checking for the next level
if (!edu.cmu.ri.terk)
   {
   edu.cmu.ri.terk = {};
   }
else
   {
   if (typeof edu.cmu.ri.terk != "object")
      {
      var eduCmuRiTerkExistsMessage = "Error: failed to create edu.cmu.ri.terk namespace: edu.cmu.ri.terk already exists and is not an object";
      alert(eduCmuRiTerkExistsMessage);
      throw new Error(eduCmuRiTerkExistsMessage);
      }
   }
//======================================================================================================================

//======================================================================================================================
// DEPENDECIES
//======================================================================================================================
if (!window['$'])
   {
   var noJQuery = "The jQuery library is required by edu.cmu.ri.terk.LCDTab.js";
   alert(noJQuery);
   throw new Error(noJQuery);
   }
//======================================================================================================================
if (!edu.cmu.ri.terk.NonClosableWaitDialog)
   {
   var noNonClosableWaitDialog = "The edu.cmu.ri.terk.NonClosableWaitDialog library is required by edu.cmu.ri.terk.LCDTab.js";
   alert(noNonClosableWaitDialog);
   throw new Error(noNonClosableWaitDialog);
   }
//======================================================================================================================
if (!edu.cmu.ri.terk.SingleButtonModalDialog)
   {
   var noSingleButtonModalDialog = "The edu.cmu.ri.terk.SingleButtonModalDialog library is required by edu.cmu.ri.terk.LCDTab.js";
   alert(noSingleButtonModalDialog);
   throw new Error(noSingleButtonModalDialog);
   }
//======================================================================================================================

//======================================================================================================================
// CODE
//======================================================================================================================
(function()
   {
   // ==================================================================================================================
   var jQuery = window['$'];
   var GUI_ALWAYS_ON_VALUE = 35;
   var JSON_ALWAYS_ON_VALUE = -1;

   edu.cmu.ri.terk.LCDTab = function(lcdConfigManager)
      {
      jQuery("#timeoutSlider")['slider']({
         animate: true,
         range: "min",
         value: 5,
         min: 0,
         max: GUI_ALWAYS_ON_VALUE,
         step: 5,
         slide: function(event, ui)
            {
            setTextDescriptionOfValue(ui.value);
            lcdConfigManager.setBacklightTimeout(ui.value == GUI_ALWAYS_ON_VALUE ? -1 : ui.value);
            }
      });

      var setTextDescriptionOfValue = function(backlightTimeout)
         {
         if (backlightTimeout == 0)
            {
            jQuery("#backlightTimeout").text("Always Off");
            }
         else
            {
            if (backlightTimeout == GUI_ALWAYS_ON_VALUE || backlightTimeout == JSON_ALWAYS_ON_VALUE)
               {
               jQuery("#backlightTimeout").text("Always On");
               }
            else
               {
               jQuery("#backlightTimeout").text(backlightTimeout);
               }
            }
         };

      // add mouse event handlers to the Save button
      jQuery('#saveLCDConfigButton').mousecapture({
         "down": function()
            {
            if (lcdConfigManager.isModified())
               {
               jQuery(this).addClass('ui-state-active');
               }
            },
         "up": function()
            {
            if (lcdConfigManager.isModified())
               {
               jQuery(this).removeClass('ui-state-active');
               }
            }}).click(
            function()
               {
               if (lcdConfigManager.isModified())
                  {
                  lcdConfigManager.saveLCDConfig();
                  }
               }
            ).disableSelection();

      var errorOccurredDialog = new edu.cmu.ri.terk.SingleButtonModalDialog(
            "Error",
            "Sorry, an error occurred while saving your preferences.  Please try again.",
            "OK");
      errorOccurredDialog.addEventListener({
         "onButtonClick": function()
            {
            errorOccurredDialog.hide();
            }
      });

      var nonClosableWaitDialog = new edu.cmu.ri.terk.NonClosableWaitDialog(
            "Saving Preferences",
            "Please wait while your changes to the LCD configuration are being saved.");

      // register the event listener
      lcdConfigManager.addEventListener({
         onBeforeLoad : function()
            {
            jQuery("#lcdConfigurationMessageArea").html("Loading preferences...");
            jQuery("#lcdConfigurationArea").hide();
            },
         onLoadSuccess : function()
            {
            var backlightTimeout = lcdConfigManager.getBacklightTimeout();
            if (backlightTimeout == JSON_ALWAYS_ON_VALUE)
               {
               backlightTimeout = GUI_ALWAYS_ON_VALUE;
               }
            jQuery("#timeoutSlider")['slider']('value', backlightTimeout);
            setTextDescriptionOfValue(backlightTimeout);
            jQuery("#lcdConfigurationMessageArea").empty();
            jQuery("#lcdConfigurationArea").show();
            },
         onLoadFailure : function()
            {
            jQuery("#lcdConfigurationMessageArea").html("Sorry, the LCD configuration is currently unavailable.");
            },
         isModified : function(isModified)
            {
            // toggle the state of the save button according to whether the config has been modified
            jQuery("#saveLCDConfigButton").toggleClass("ui-state-default", isModified);
            jQuery("#saveLCDConfigButton").toggleClass("ui-state-active", !isModified);
            jQuery("#saveLCDConfigButton").toggleClass("ui-state-disabled", !isModified);
            if (isModified)
               {
               jQuery("#lcdNoteAboutConfigChanges").show();
               }
            else
               {
               jQuery("#lcdNoteAboutConfigChanges").hide();
               }
            },
         onBeforeSave: function()
            {
            nonClosableWaitDialog.show();
            },
         onSaveSuccess: function()
            {
            nonClosableWaitDialog.hide();
            },
         onSaveFailure: function()
            {
            nonClosableWaitDialog.hide();
            errorOccurredDialog.show();
            }});

      this.activate = function()
         {
         lcdConfigManager.getLCDConfig();
         };
      // ---------------------------------------------------------------------------------------------------------------
      };

   // ==================================================================================================================
   })();
