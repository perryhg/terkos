//======================================================================================================================
// Class for initializing and configuring the various widgets on the System Settings tab.
//
// Dependencies:
// * jQuery (http://jquery.com/)
// * jQuery UI (http://jqueryui.com/)
// * jquery.mousecapture.js (http://benanne.net/code/?p=238)
// * ajaxupload.3.5.js (http://valums.com/ajax-upload/)
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
      // ---------------------------------------------------------------------------------------------------------------
      // Common

      var notifyEventListeners = function(eventName, listeners)
         {
         jQuery.each(listeners, function(i, listener)
            {
            if (listener && listener[eventName])
               {
               listener[eventName]();
               }
            });
         };

      var addEventListener = function(listener, listeners)
         {
         if (listener)
            {
            listeners[listeners.length] = listener;
            }
         };

      // ---------------------------------------------------------------------------------------------------------------
      // Backup Preferences

      // add mouse event handlers to the Backup button
      jQuery('#backupPreferencesButton').mousecapture({
         "down": function()
            {
            jQuery(this).addClass('ui-state-active');
            },
         "up": function()
            {
            jQuery(this).removeClass('ui-state-active');
            }}).click(
            function()
               {
               location.href = host + '/cgi-bin/getTerkOSConfig.pl';
               }
            ).disableSelection();

      // ---------------------------------------------------------------------------------------------------------------
      // Restore Preferences

      var isRestoreFileSelected = false;

      var confirmRestorePreferencesDialog = new edu.cmu.ri.terk.TwoButtonModalDialog(
            "Restore Confirmation",
            "Are you sure you want to restore the preferences?",
            "OK",
            "Cancel");

      confirmRestorePreferencesDialog.addEventListener({
         "onButton1Click": function()
            {
            confirmRestorePreferencesDialog.hide();

            ajaxUpload.submit();
            },
         "onButton2Click": function()
            {
            confirmRestorePreferencesDialog.hide();
            }
      });

      var restorePreferencesNonClosableWaitDialog = new edu.cmu.ri.terk.NonClosableWaitDialog(
            "Restoring Preferences",
            "Please wait while the preferences are being restored.");

      var restorePreferencesErrorOccurredDialog = new edu.cmu.ri.terk.SingleButtonModalDialog(
            "Error",
            "Sorry, an error occurred while restoring your preferences.  Please try again.",
            "OK");

      restorePreferencesErrorOccurredDialog.addEventListener({
         "onButtonClick": function()
            {
            restorePreferencesErrorOccurredDialog.hide();
            }
      });

      var restorePreferencesOkDialog = new edu.cmu.ri.terk.SingleButtonModalDialog(
            "Restore Successful",
            "The restore completed successfully.",
            "OK");

      restorePreferencesOkDialog.addEventListener({
         "onButtonClick": function()
            {
            restorePreferencesOkDialog.hide();
            }
      });

      var ajaxUpload = new AjaxUpload('#restorePreferencesFileChooserButton', {
         // Location of the server-side upload script
         // NOTE: You are not allowed to upload files to another domain
         action: host + '/cgi-bin/setTerkOSConfig.pl',
         // File upload name
         name: 'configFile',
         // Submit file after selection
         autoSubmit: false,
         // The type of data that you're expecting back from the server.
         // HTML (text) and XML are detected automatically.
         // Useful when you are using JSON data as a response, set to "json" in that case.
         // Also set server response type to text/html, otherwise it will not work in IE6
         responseType: "json",
         // Fired after the file is selected
         // Useful when autoSubmit is disabled
         // You can return false to cancel upload
         // @param file basename of uploaded file
         // @param extension of that file
         onChange: function(file, extension)
            {
            jQuery("#selectedFileToUpload").text(file);
            isRestoreFileSelected = (file != null && file != "");
            updateRestoreButtonState(isRestoreFileSelected);
            },
         // Fired before the file is uploaded
         // You can return false to cancel upload
         // @param file basename of uploaded file
         // @param extension of that file
         onSubmit: function(file, extension)
            {
            restorePreferencesNonClosableWaitDialog.show();
            },
         // Fired when file upload is completed
         // WARNING! DO NOT USE "FALSE" STRING AS A RESPONSE!
         // @param file basename of uploaded file
         // @param response server response
         onComplete: function(file, response)
            {
            jQuery("#selectedFileToUpload").text('[unspecified]');
            updateRestoreButtonState(false);
            restorePreferencesNonClosableWaitDialog.hide();

            var isOK = response['ok'] &&
                       response['status_detail'] &&
                       response['status_detail']['ok'] &&
                       response['status_detail']['updated-components'] &&
                       response['status_detail']['updated-components'].length > 0;
            if (isOK)
               {
               restorePreferencesOkDialog.show();
               }
            else
               {
               if (response['message'] == 'invalid JSON' || response['message'].indexOf('required JSON element missing') == 0)
                  {
                  restorePreferencesErrorOccurredDialog.setMessage("Sorry, an error occurred while trying to restore your preferences.  The preferences file you uploaded is not valid.");
                  }
               else if (response['message'] == 'unknown firmware revision')
                  {
                  restorePreferencesErrorOccurredDialog.setMessage("Sorry, an error occurred while trying to restore your preferences.  The preferences file you uploaded appears to be for a firmware version which is newer than the currently running version.");
                  }
               else
                  {
                  restorePreferencesErrorOccurredDialog.setMessage("Sorry, an error occurred while restoring your preferences.  Please try again.");
                  }
               restorePreferencesErrorOccurredDialog.show();
               }
            }
      });

      var updateRestoreButtonState = function(isEnabled)
         {
         // toggle the state of the reset button according to whether the config has been modified
         jQuery("#restorePreferencesButton").toggleClass("ui-state-default", isEnabled);
         jQuery("#restorePreferencesButton").toggleClass("ui-state-active", !isEnabled);
         jQuery("#restorePreferencesButton").toggleClass("ui-state-disabled", !isEnabled);
         };

      // add mouse event handlers to the Restore button
      jQuery('#restorePreferencesButton').mousecapture({
         "down": function()
            {
            if (isRestoreFileSelected)
               {
               jQuery(this).addClass('ui-state-active');
               }
            },
         "up": function()
            {
            if (isRestoreFileSelected)
               {
               jQuery(this).removeClass('ui-state-active');
               }
            }}).click(
            function()
               {
               if (isRestoreFileSelected)
                  {
                  confirmRestorePreferencesDialog.show();
                  }
               }
            ).disableSelection();

      // ---------------------------------------------------------------------------------------------------------------
      // Reset Preferences

      var resetPreferencesEventListeners = new Array();

      var resetPreferencesNonClosableWaitDialog = new edu.cmu.ri.terk.NonClosableWaitDialog(
            "Resetting Preferences",
            "Please wait while the preferences are being reset.");

      var resetPreferencesErrorOccurredDialog = new edu.cmu.ri.terk.SingleButtonModalDialog(
            "Error",
            "Sorry, an error occurred while resetting your preferences.  Please try again.",
            "OK");

      resetPreferencesErrorOccurredDialog.addEventListener({
         "onButtonClick": function()
            {
            resetPreferencesErrorOccurredDialog.hide();
            }
      });

      var webPasswordOkDialog = new edu.cmu.ri.terk.SingleButtonModalDialog(
            "Reset Successful",
            "The reset completed successfully.  Since you chose to reset the web control panel password, the page will reload and you may need to log in again.",
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
               notifyResetPreferencesEventListeners('onBeforeReset');

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
                        notifyResetPreferencesEventListeners('onResetSuccess');
                        }
                     else
                        {
                        notifyResetPreferencesEventListeners('onResetFailure');
                        }
                     },
                  error: function()
                     {
                     notifyResetPreferencesEventListeners('onResetFailure');
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

         // toggle the state of the reset button according to whether the config has been modified
         jQuery("#resetPreferencesButton").toggleClass("ui-state-default", isEnabled);
         jQuery("#resetPreferencesButton").toggleClass("ui-state-active", !isEnabled);
         jQuery("#resetPreferencesButton").toggleClass("ui-state-disabled", !isEnabled);
         };

      var notifyResetPreferencesEventListeners = function(eventName)
         {
         notifyEventListeners(eventName, resetPreferencesEventListeners);
         };

      /**
       * Registers the given event listener for resetting preferences.  The event listener should implement some or all
       * of the following methods:
       *
       *    onBeforeReset()
       *    onResetSuccess()
       *    onResetFailure()
       */
      var addResetPreferencesEventListener = function(listener)
         {
         addEventListener(listener, resetPreferencesEventListeners);
         };

      // register the event listener
      addResetPreferencesEventListener({
         onBeforeReset: function()
            {
            resetPreferencesNonClosableWaitDialog.show();
            },
         onResetSuccess: function()
            {
            var wasPasswordReset = jQuery("#willResetWebControlPanelPassword").attr("checked");

            jQuery("#willResetAll").attr("checked", "");
            jQuery(".reset-preferences-option").attr("checked", "");
            updateResetButtonState();

            resetPreferencesNonClosableWaitDialog.hide();
            if (wasPasswordReset)
               {
               webPasswordOkDialog.show();
               }
            },
         onResetFailure: function()
            {
            resetPreferencesNonClosableWaitDialog.hide();
            resetPreferencesErrorOccurredDialog.show();
            }});

      // add mouse event handlers to the Reset button
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
                  if (jQuery("#willResetWebControlPanelPassword").attr("checked"))
                     {
                     confirmResetDialog.setMessage(defaultConfirmResetDialogMessage +
                                                   "<br><br>Note that since you have chosen to reset the web control panel password you may need to log in again after the reset is complete.");
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
