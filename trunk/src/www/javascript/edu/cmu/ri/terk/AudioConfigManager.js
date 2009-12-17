//======================================================================================================================
// This file is part of Terk and TerkOS.
//
// All Terk and TerkOS source code is provided under the terms of the GNU General Public License v2
// (http://www.gnu.org/licenses/gpl-2.0.html). Those wishing to use Terk and TerkOS source code, software and/or
// technologies under different licensing terms should contact us at telepresencerobotkit@cs.cmu.edu. Such licensing
// terms are available for all portions of the Terk and TerkOS codebase presented here.
//======================================================================================================================
// Class for managing the audio configuration.
//
// Dependencies:
// * jQuery (http://jquery.com/)
// * jQuery UI (http://jqueryui.com/)
// * json2.js (http://www.JSON.org/json2.js)
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
   var noJQuery = "The jQuery library is required by edu.cmu.ri.terk.AudioConfigManager.js";
   alert(noJQuery);
   throw new Error(noJQuery);
   }
//======================================================================================================================
if (!JSON)
   {
   var msg3 = "The json2.js library is required by edu.cmu.ri.terk.AudioConfigManager.js";
   alert(msg3);
   throw new Error(msg3);
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

   edu.cmu.ri.terk.AudioConfigManager = function(host)
      {
      var eventListeners = new Array();
      var stringifiedOriginalJSON = null;
      var json = null;

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

      this.getAudioConfig = function()
         {
         notifyEventListeners('onBeforeLoad');

         loadAudioConfig();
         };

      var loadAudioConfig = function(onSuccessCallbackFunction)
         {
         // load the config
         jQuery.ajax(
         {
            url: host + '/cgi-bin/getAudioConfigAsJSON.pl',
            success: function(jsonResponse)
               {
               json = jsonResponse;
               stringifiedOriginalJSON = JSON.stringify(json);

               if (json)
                  {
                  notifyEventListeners('onLoadSuccess');
                  }
               else
                  {
                  notifyEventListeners('onLoadFailure');
                  }

               notifyChangeListeners();
               if (onSuccessCallbackFunction)
                  {
                  onSuccessCallbackFunction();
                  }
               },
            error: function()
               {
               notifyEventListeners('onLoadFailure');
               json = null;
               stringifiedOriginalJSON = null;
               }
         });
         };

      this.saveAudioConfig = function()
         {
         if (this.isModified())
            {
            notifyEventListeners('onBeforeSave');

            // send the JSON to the backend
            jQuery.ajax(
            {
               url: host + '/cgi-bin/saveAudioConfig.pl',
               type: 'POST',
               data: "json=" + JSON.stringify(json),
               success: function(jsonResponse)
                  {
                  if (jsonResponse)
                     {
                     // reload the config
                     loadAudioConfig(function()
                        {
                        notifyEventListeners('onSaveSuccess');
                        });
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
         };

      this.playSampleSound = function(volume)
         {
         // send the JSON to the backend
         jQuery.ajax(
         {
            url: host + '/cgi-bin/playSampleSound.pl',
            type: 'POST',
            data: "volume=" + volume
         });
         };

      var getIsModified = function()
         {
         return (JSON.stringify(json) != stringifiedOriginalJSON);
         };

      this.isModified = function()
         {
         return getIsModified();
         };

      var notifyChangeListeners = function()
         {
         // compare stringified versions of the original JSON with the updated one to see if
         // anything has changed and then update the listeners accordingly
         var isModified = getIsModified();

         // notify event listeners of the modification state
         jQuery.each(eventListeners, function(i, listener)
            {
            if (listener && listener['isModified'])
               {
               listener['isModified'](isModified);
               }
            });
         };

      /**
       * Registers the given event listener.  The event listener should implement some or all of the following methods:
       *
       *    onBeforeLoad()
       *    onLoadSuccess()
       *    onLoadFailure()
       *    isModified(boolean)
       *    onBeforeSave()
       *    onSaveSuccess()
       *    onSaveFailure()
       */
      this.addEventListener = function(listener)
         {
         if (listener)
            {
            eventListeners[eventListeners.length] = listener;
            }
         };

      var getJSONProperty = function(jsonRoot, propertyPath)
         {
         if (jsonRoot)
            {
            if (propertyPath)
               {
               var propertyNames = propertyPath.split('.');
               for (var i = 0; i < propertyNames.length; i++)
                  {
                  var propertyName = propertyNames[i];
                  if (typeof jsonRoot[propertyName] == 'undefined')
                     {
                     return null;
                     }
                  else
                     {
                     jsonRoot = jsonRoot[propertyName];
                     }
                  }
               return jsonRoot;
               }

            return jsonRoot;
            }

         return null;
         };

      this.setAudioAlertsEnabled = function(isEnabled)
         {
         json['audio']['alerts']['enabled'] = isEnabled;
         notifyChangeListeners();
         };

      this.setVolume = function(volume)
         {
         json['audio']['volume']['level'] = volume;
         notifyChangeListeners();
         };

      this.isAudioAlertsEnabled = function()
         {
         return getJSONProperty(json, "audio.alerts.enabled");
         };

      this.getVolume = function()
         {
         return getJSONProperty(json, "audio.volume.level");
         };
      // ---------------------------------------------------------------------------------------------------------------
      };

   // ==================================================================================================================
   })();

