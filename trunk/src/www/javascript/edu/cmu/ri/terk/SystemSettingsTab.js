//======================================================================================================================
// Class for initializing and configuring the various widgets on the System Settings tab.
//
// Dependencies:
// * jQuery (http://jquery.com/)
// * jQuery UI (http://jqueryui.com/)
// * jquery.mousecapture.js (http://benanne.net/code/?p=238)
// * edu.cmu.ri.terk.NonClosableWaitDialog
// * edu.cmu.ri.terk.SingleButtonModalDialog
// * edu.cmu.ri.terk.TwoButtonModalDialog
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
   var noJQuery = "The jQuery library is required by edu.cmu.ri.terk.SystemSettingsTab.js";
   alert(noJQuery);
   throw new Error(noJQuery);
   }
//======================================================================================================================
if (!edu.cmu.ri.terk.NonClosableWaitDialog)
   {
   var noNonClosableWaitDialog = "The edu.cmu.ri.terk.NonClosableWaitDialog library is required by edu.cmu.ri.terk.SystemSettingsTab.js";
   alert(noNonClosableWaitDialog);
   throw new Error(noNonClosableWaitDialog);
   }
//======================================================================================================================
if (!edu.cmu.ri.terk.SingleButtonModalDialog)
   {
   var noSingleButtonModalDialog = "The edu.cmu.ri.terk.SingleButtonModalDialog library is required by edu.cmu.ri.terk.SystemSettingsTab.js";
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
   jQuery.ajaxSetup({
      type: 'GET',
      dataType: 'jsonp',
      timeout: 10000,
      cache: false,
      global: false
   });

   edu.cmu.ri.terk.SystemSettingsTab = function(host)
      {
      var eventListeners = new Array();

      var nonClosableWaitDialog = new edu.cmu.ri.terk.NonClosableWaitDialog(
            "Resetting Preferences",
            "Please wait while the preferences are being reset.");

      var errorOccurredDialog = new edu.cmu.ri.terk.SingleButtonModalDialog(
            "Error",
            "Sorry, an error occurred while resetting your preferences.  Please try again.",
            "OK");
      errorOccurredDialog.addEventListener({
         "onButtonClick": function()
            {
            errorOccurredDialog.hide();
            }
      });

      var webPasswordOkDialog = new edu.cmu.ri.terk.SingleButtonModalDialog(
            "Reset Successful",
            "The reset completed successfully.  Since you chose to reset the web control panel password, you may need to log in again.",
            "OK");
      webPasswordOkDialog.addEventListener({
         "onButtonClick": function()
            {
            webPasswordOkDialog.hide();
            window.location.reload();
            }
      });

      var defaultConfirmResetDialogMessage = "Are you sure you want to reset the selected preferences?";
      var confirmResetDialog = new edu.cmu.ri.terk.TwoButtonModalDialog(
            "Reset Confirmation",
            defaultConfirmResetDialogMessage,
            "OK",
            "Cancel");
      confirmResetDialog.addEventListener({
         "onButton1Click": function()
            {
            confirmResetDialog.hide();

            // build an array of the selected reset options
            var resetOptions = new Array();
            jQuery(".reset-preferences-option:checked").each(function(id, val)
               {
               resetOptions.push(jQuery(val).val());
               });

            // if the array is non-empty (it should be!), then make the AJAX call
            if (resetOptions.length > 0)
               {
               notifyEventListeners('onBeforeSave');

               // send the JSON to the backend
               jQuery.ajax(
               {
                  url: host + '/cgi-bin/resetPreferences.pl',
                  type: 'POST',
                  data: "systemsToReset=" + resetOptions.join(','),
                  success: function(jsonResponse)
                     {
                     if (jsonResponse)
                        {
                        notifyEventListeners('onSaveSuccess');
                        }
                     else
                        {
                        notifyEventListeners('onSaveFailure');
                        }
                     },
                  error: function()
                     {
                     notifyEventListeners('onSaveFailure');
                     }
               });
               }
            },
         "onButton2Click": function()
            {
            confirmResetDialog.hide();
            }
      });

      jQuery(".reset-preferences-option")['change'](function()
         {
         var numOptions = jQuery(".reset-preferences-option").length;
         var numChecked = jQuery(".reset-preferences-option:checked").length;
         jQuery("#willResetAll").attr("checked", numOptions == numChecked ? "checked" : "");
         updateResetButtonState();
         });

      jQuery("#willResetAll")['change'](function()
         {
         jQuery(".reset-preferences-option").attr("checked", jQuery(this).attr("checked") ? "checked" : "");
         updateResetButtonState();
         });

      var isAtLeastOneResetOptionChecked = function()
         {
         return jQuery(".reset-preferences-option:checked").length > 0;
         };

      var updateResetButtonState = function()
         {
         var isEnabled = isAtLeastOneResetOptionChecked();

         // toggle the state of the save button according to whether the config has been modified
         jQuery("#resetPreferencesButton").toggleClass("ui-state-default", isEnabled);
         jQuery("#resetPreferencesButton").toggleClass("ui-state-active", !isEnabled);
         jQuery("#resetPreferencesButton").toggleClass("ui-state-disabled", !isEnabled);
         };

      var notifyEventListeners = function(eventName)
         {
         jQuery.each(eventListeners, function(i, listener)
            {
            if (listener && listener[eventName])
               {
               listener[eventName]();
               }
            });
         };

      /**
       * Registers the given event listener.  The event listener should implement some or all of the following methods:
       *
       *    onBeforeSave()
       *    onSaveSuccess()
       *    onSaveFailure()
       */
      var addEventListener = function(listener)
         {
         if (listener)
            {
            eventListeners[eventListeners.length] = listener;
            }
         };

      // register the event listener
      addEventListener({
         onBeforeSave: function()
            {
            nonClosableWaitDialog.show();
            },
         onSaveSuccess: function()
            {
            var wasPasswordReset = jQuery("#willResetPasswords").attr("checked");

            jQuery("#willResetAll").attr("checked", "");
            jQuery(".reset-preferences-option").attr("checked", "");
            updateResetButtonState();

            nonClosableWaitDialog.hide();
            if (wasPasswordReset)
               {
               webPasswordOkDialog.show();
               }
            },
         onSaveFailure: function()
            {
            nonClosableWaitDialog.hide();
            errorOccurredDialog.show();
            }});

      // add mouse event handlers to the Save button
      jQuery('#resetPreferencesButton').mousecapture({
         "down": function()
            {
            if (isAtLeastOneResetOptionChecked())
               {
               jQuery(this).addClass('ui-state-active');
               }
            },
         "up": function()
            {
            if (isAtLeastOneResetOptionChecked())
               {
               jQuery(this).removeClass('ui-state-active');
               }
            }}).click(
            function()
               {
               if (isAtLeastOneResetOptionChecked())
                  {
                  // give the user a warning about having to login again if they're
                  // resetting the web control panel password.
                  if (jQuery("#willResetPasswords").attr("checked"))
                     {
                     confirmResetDialog.setMessage(defaultConfirmResetDialogMessage +
                                                   "<br><br>Note that since you have chosen to reset the web control panel password you will need to log in again after the reset is complete.");
                     }
                  else
                     {
                     confirmResetDialog.setMessage(defaultConfirmResetDialogMessage);
                     }

                  confirmResetDialog.show();
                  }
               }
            ).disableSelection();

      this.activate = function()
         {
         // nothing to do
         };
      // ---------------------------------------------------------------------------------------------------------------
      };

   // ==================================================================================================================
   })();
