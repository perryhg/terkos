//======================================================================================================================
// Class for initializing and configuring the various widgets on the Passwords tab.
//
// Dependencies:
// * jQuery (http://jquery.com/)
// * jQuery UI (http://jqueryui.com/)
// * jquery.mousecapture.js (http://benanne.net/code/?p=238)
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
   var noJQuery = "The jQuery library is required by edu.cmu.ri.terk.PasswordsTab.js";
   alert(noJQuery);
   throw new Error(noJQuery);
   }
//======================================================================================================================
if (!edu.cmu.ri.terk.SingleButtonModalDialog)
   {
   var noSingleButtonModalDialog = "The edu.cmu.ri.terk.SingleButtonModalDialog library is required by edu.cmu.ri.terk.PasswordsTab.js";
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
      timeout: 3000,
      cache: false,
      global: false
   });

   edu.cmu.ri.terk.PasswordsTab = function(dialogManager, host)
      {
      var PasswordVerifier = function(
            passwordFieldId,
            passwordConfirmationFieldId,
            messageAreaId,
            saveButtonId,
            nonMatchingPasswordsErrorMessage,
            getAjaxUrlAndData,
            additionalValidationFunction,
            additionalFieldsToValidate)
         {
         var isPasswordValid = false;
         var eventListeners = new Array();

         this.addEventListener = function(listener)
            {
            if (listener)
               {
               eventListeners[eventListeners.length] = listener;
               }
            };

         var notifyEventListeners = function(eventName, state)
            {
            jQuery.each(eventListeners, function(i, listener)
               {
               if (listener && listener[eventName])
                  {
                  listener[eventName](state);
                  }
               });
            };

         var validatePasswords = function()
            {
            var p1 = jQuery(passwordFieldId).val();
            var p2 = jQuery(passwordConfirmationFieldId).val();
            var areBothNonEmpty = (p1.length > 0) && (p2.length > 0);
            var areEqual = (p1 == p2);
            var additionalValidation = true;
            if (additionalValidationFunction)
               {
               additionalValidation = additionalValidationFunction();
               }
            notifyEventListeners('non-empty', areBothNonEmpty);
            notifyEventListeners('equal', areEqual);
            notifyEventListeners('valid', areEqual && areBothNonEmpty && additionalValidation);
            };

         var setSaveButtonEnabled = function(buttonId, isEnabled)
            {
            jQuery(buttonId).toggleClass("ui-state-default", isEnabled);
            jQuery(buttonId).toggleClass("ui-state-active", !isEnabled);
            jQuery(buttonId).toggleClass("ui-state-disabled", !isEnabled);
            };

         jQuery(passwordFieldId)['keyup'](function()
            {
            validatePasswords();
            });

         jQuery(passwordConfirmationFieldId)['keyup'](function()
            {
            validatePasswords();
            });

         if (additionalFieldsToValidate && additionalFieldsToValidate.length > 0)
            {
            jQuery.each(additionalFieldsToValidate, function(i, additionalFieldToValidate)
               {
               if (additionalFieldToValidate)
                  {
                  jQuery(additionalFieldToValidate)['keyup'](function()
                     {
                     validatePasswords();
                     });
                  }
               });
            }

         this.addEventListener({
            "non-empty" : function(isNonEmpty)
               {
               if (isNonEmpty)
                  {
                  jQuery(messageAreaId).show();
                  }
               else
                  {
                  jQuery(messageAreaId).hide();
                  }
               },
            "equal" : function(isEqual)
               {
               if (isEqual)
                  {
                  jQuery(messageAreaId).text("");
                  }
               else
                  {
                  jQuery(messageAreaId).addClass("status-fault");
                  jQuery(messageAreaId).html(nonMatchingPasswordsErrorMessage);
                  }
               },
            "valid" : function(isValid)
               {
               isPasswordValid = isValid;

               // toggle the state of the save button according to whether the passwords are valid
               setSaveButtonEnabled(saveButtonId, isValid);
               }
         });

         // add mouse event handlers to the Save button
         jQuery(saveButtonId).mousecapture({
            "down": function()
               {
               if (isPasswordValid)
                  {
                  jQuery(this).addClass('ui-state-active');
                  }
               },
            "up": function()
               {
               if (isPasswordValid)
                  {
                  jQuery(this).removeClass('ui-state-active');
                  }
               }}).click(
               function()
                  {
                  if (isPasswordValid)
                     {
                     notifyEventListeners('onBeforeSave');

                     // get the ajax URL and data fields
                     var urlAndData = getAjaxUrlAndData();

                     // send the JSON to the backend
                     jQuery.ajax(
                     {
                        url: urlAndData['url'],
                        type: 'POST',
                        data: urlAndData['data'],
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
                  }
               ).disableSelection();
         };

      var webPasswordOkDialog = new edu.cmu.ri.terk.SingleButtonModalDialog(
            "Password Saved",
            "The new web control panel password was saved successfully.  You will now need to log in again.",
            "OK");
      webPasswordOkDialog.addEventListener({
         "onButtonClick": function()
            {
            webPasswordOkDialog.hide();
            window.location.reload();
            }
      });

      var webPasswordVerifier = new PasswordVerifier(
            "#passwordsNewWebPassword",
            "#passwordsNewWebPasswordConfirmation",
            "#editWebPasswordMessage",
            "#saveNewWebPasswordButton",
            "the passwords do not match",
            function()
               {
               var urlAndData = {
                  url: host + '/cgi-bin/setHttpBasicAuthPassword.pl',
                  data: "newPassword=" + jQuery("#passwordsNewWebPassword").val()
               };

               return urlAndData;
               }
            );
      webPasswordVerifier.addEventListener({
         "onBeforeSave" : function()
            {
            dialogManager.showNonClosableWaitDialog("Please wait while the web control panel password is being saved.");
            },
         "onSaveSuccess" : function()
            {
            dialogManager.hideNonClosableWaitDialog();
            webPasswordOkDialog.show()
            },
         "onSaveFailure" : function()
            {
            dialogManager.hideNonClosableWaitDialog();
            dialogManager.showOKDialog("Sorry, an error occurred while saving the web control panel password.  Please try again.");
            }
      });

      var rootPasswordVerifier = new PasswordVerifier(
            "#passwordsNewRootUserPassword",
            "#passwordsNewRootUserPasswordConfirmation",
            "#editRootUserPasswordMessage",
            "#saveNewRootUserPasswordButton",
            "the new passwords do not match",
            function()
               {
               var urlAndData = {
                  url: host + '/cgi-bin/setHttpBasicAuthPassword.pl',
                  data: "newPassword=" + jQuery("#passwordsNewRootUserPassword").val()
               };

               return urlAndData;
               },
            function()
               {
               var length = jQuery("#passwordsCurrentRootUserPassword").val().length;
               return (length > 0);
               },
            jQuery([]).add(jQuery("#passwordsCurrentRootUserPassword"))
            );
      rootPasswordVerifier.addEventListener({
         "onBeforeSave" : function()
            {
            dialogManager.showNonClosableWaitDialog("Please wait while the root user password is being saved.");
            },
         "onSaveSuccess" : function()
            {
            dialogManager.hideNonClosableWaitDialog();
            dialogManager.showOKDialog("The new root password was saved successfully.");
            },
         "onSaveFailure" : function()
            {
            dialogManager.hideNonClosableWaitDialog();
            dialogManager.showOKDialog("Sorry, an error occurred while saving the root user password.  Please try again.");
            }
      });

      this.activate = function()
         {
         // nothing to do
         };
      // ---------------------------------------------------------------------------------------------------------------
      };

   // ==================================================================================================================
   })
      ();
