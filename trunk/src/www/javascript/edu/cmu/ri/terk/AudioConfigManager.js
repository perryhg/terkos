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
   var noJQuery = "The jQuery library is required by edu.cmu.ri.terk.AudioTab.js";
   alert(noJQuery);
   throw new Error(noJQuery);
   }
//======================================================================================================================
if (!JSON)
   {
   var msg3 = "The json2.js library is required by edu.cmu.ri.terk.AudioTab.js";
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
      timeout: 3000,
      cache: false,
      global: false
   });

   edu.cmu.ri.terk.AudioConfigManager = function()
      {
      var host = '';//http://192.168.1.4'; // TODO: remove me!

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

      this.setAudioAlertsEnabled = function(isEnabled)
         {
         json['alerts']['enabled'] = isEnabled;
         notifyChangeListeners();
         };

      this.setVolume = function(volume)
         {
         json['volume']['level'] = volume;
         notifyChangeListeners();
         };

      this.isAudioAlertsEnabled = function()
         {
         return json['alerts']['enabled'];
         };

      this.getVolume = function()
         {
         return json['volume']['level'];
         };
      // ---------------------------------------------------------------------------------------------------------------
      };

   // ==================================================================================================================
   })();

