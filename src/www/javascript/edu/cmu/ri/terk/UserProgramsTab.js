//======================================================================================================================
// Class for initializing and configuring the various widgets on the User Programs tab.
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
else if (typeof edu != "object")
   {
   var eduExistsMessage = "Error: failed to create edu namespace: edu already exists and is not an object";
   alert(eduExistsMessage);
   throw new Error(eduExistsMessage);
   }

// Repeat the creation and type-checking for the next level
if (!edu.cmu)
   {
   edu.cmu = {};
   }
else if (typeof edu.cmu != "object")
   {
   var eduCmuExistsMessage = "Error: failed to create edu.cmu namespace: edu.cmu already exists and is not an object";
   alert(eduCmuExistsMessage);
   throw new Error(eduCmuExistsMessage);
   }

// Repeat the creation and type-checking for the next level
if (!edu.cmu.ri)
   {
   edu.cmu.ri = {};
   }
else if (typeof edu.cmu.ri != "object")
   {
   var eduCmuRiExistsMessage = "Error: failed to create edu.cmu.ri namespace: edu.cmu.ri already exists and is not an object";
   alert(eduCmuRiExistsMessage);
   throw new Error(eduCmuRiExistsMessage);
   }

// Repeat the creation and type-checking for the next level
if (!edu.cmu.ri.terk)
   {
   edu.cmu.ri.terk = {};
   }
else if (typeof edu.cmu.ri.terk != "object")
   {
   var eduCmuRiTerkExistsMessage = "Error: failed to create edu.cmu.ri.terk namespace: edu.cmu.ri.terk already exists and is not an object";
   alert(eduCmuRiTerkExistsMessage);
   throw new Error(eduCmuRiTerkExistsMessage);
   }
//======================================================================================================================

//======================================================================================================================
// DEPENDECIES
//======================================================================================================================
if (!window['$'])
   {
   var noJQuery = "The jQuery library is required by edu.cmu.ri.terk.UserProgramsTab.js";
   alert(noJQuery);
   throw new Error(noJQuery);
   }
//======================================================================================================================
if (!edu.cmu.ri.terk.NonClosableWaitDialog)
   {
   var noNonClosableWaitDialog = "The edu.cmu.ri.terk.NonClosableWaitDialog library is required by edu.cmu.ri.terk.UserProgramsTab.js";
   alert(noNonClosableWaitDialog);
   throw new Error(noNonClosableWaitDialog);
   }
//======================================================================================================================
if (!edu.cmu.ri.terk.SingleButtonModalDialog)
   {
   var noSingleButtonModalDialog = "The edu.cmu.ri.terk.SingleButtonModalDialog library is required by edu.cmu.ri.terk.UserProgramsTab.js";
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

   edu.cmu.ri.terk.UserProgramsTab = function(userProgramsConfigManager)
      {
      // configure the select list
      jQuery('#userProgramName')['change'](
            function()
               {
               userProgramsConfigManager.setProgramToRunOnStartup(jQuery(this).val());
               }
            );

      // add mouse event handlers to the Save button
      jQuery('#saveUserProgramConfigButton').mousecapture({
         "down": function()
            {
            if (userProgramsConfigManager.isModified())
               {
               jQuery(this).addClass('ui-state-active');
               }
            },
         "up": function()
            {
            if (userProgramsConfigManager.isModified())
               {
               jQuery(this).removeClass('ui-state-active');
               }
            }}).click(
            function()
               {
               if (userProgramsConfigManager.isModified())
                  {
                  userProgramsConfigManager.saveConfig();
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
            "Please wait while your changes to the user programs configuration are being saved.");

      // register the event listener
      userProgramsConfigManager.addEventListener({
         onBeforeLoad : function()
            {
            jQuery("#userProgramConfigurationMessageArea").html("Loading preferences...");
            jQuery("#userProgramConfigurationArea").hide();
            },
         onLoadSuccess : function()
            {
            var preferredProgramName = userProgramsConfigManager.getProgramToRunOnStartup();
            jQuery("#userProgramName").empty();
            jQuery("#userProgramName").append('<option value=""></option');
            userProgramsConfigManager.programNamesIterator(function(programName)
               {
               var isSelected = (preferredProgramName == programName);
               jQuery("#userProgramName").append('<option value="' + programName + '" ' + (isSelected ? 'selected="selected"' : '') + '>' + programName + '</option>');
               });

            jQuery("#userProgramConfigurationMessageArea").empty();
            jQuery("#userProgramConfigurationArea").show();
            },
         onLoadFailure : function()
            {
            jQuery("#userProgramConfigurationMessageArea").html("Sorry, the user programs configuration is currently unavailable.");
            },
         isModified : function(isModified)
            {
            // toggle the state of the save button according to whether the config has been modified
            jQuery("#saveUserProgramConfigButton").toggleClass("ui-state-default", isModified);
            jQuery("#saveUserProgramConfigButton").toggleClass("ui-state-active", !isModified);
            jQuery("#saveUserProgramConfigButton").toggleClass("ui-state-disabled", !isModified);
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
         userProgramsConfigManager.getConfig();
         };
      // ---------------------------------------------------------------------------------------------------------------
      };

   // ==================================================================================================================
   })();
