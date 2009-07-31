//======================================================================================================================
// Class for initializing and configuring the various widgets on the Passwords tab.
//
// Dependencies:
// * jQuery (http://jquery.com/)
// * jQuery UI (http://jqueryui.com/)
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

//======================================================================================================================
// CODE
//======================================================================================================================
(function()
   {
   // ==================================================================================================================
   var jQuery = window['$'];

   edu.cmu.ri.terk.PasswordsTab = function(dialogManager)
      {
      var PasswordVerifier = function(
            passwordFieldId,
            passwordConfirmationFieldId,
            messageAreaId,
            saveButtonId,
            nonMatchingPasswordsErrorMessage,
            saveFunction,
            additionalValidationFunction,
            additionalFieldsToValidate)
         {
         var isPasswordValid = false;
         var eventListeners = new Array();

         var addEventListener = function(listener)
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

         if (additionalFieldsToValidate)
            {
            jQuery.each(additionalFieldsToValidate, function(i, additionalFieldToValidate)
               {
               additionalFieldsToValidate['keyup'](function()
                  {
                  validatePasswords();
                  });
               });
            }

         addEventListener({
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
                  if (isPasswordValid && saveFunction)
                     {
                     saveFunction();
                     }
                  }
               ).disableSelection();
         };

      var webPasswordVerifier = new PasswordVerifier(
            "#passwordsNewWebPassword",
            "#passwordsNewWebPasswordConfirmation",
            "#editWebPasswordMessage",
            "#saveNewWebPasswordButton",
            "the passwords do not match",
            function()
               {
               alert("Save web password not yet implemented.");
               }
            );

      var rootPasswordVerifier = new PasswordVerifier(
            "#passwordsNewRootUserPassword",
            "#passwordsNewRootUserPasswordConfirmation",
            "#editRootUserPasswordMessage",
            "#saveNewRootUserPasswordButton",
            "the new passwords do not match",
            function()
               {
               alert("Save root password not yet implemented.");
               },
            function()
               {
               var length = jQuery("#passwordsCurrentRootUserPassword").val().length;
               return (length > 0);
               },
            jQuery([]).add(jQuery("#passwordsCurrentRootUserPassword"))
            );

      this.activate = function()
         {
         // nothing to do
         };
      // ---------------------------------------------------------------------------------------------------------------
      };

   // ==================================================================================================================
   })();
