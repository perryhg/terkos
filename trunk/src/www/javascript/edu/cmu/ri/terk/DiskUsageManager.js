//======================================================================================================================
// Class for reading the disk usage info.
//
// Dependencies:
// * jQuery (http://jquery.com/)
// * jQuery UI (http://jqueryui.com/)
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
   var noJQuery = "The jQuery library is required by edu.cmu.ri.terk.DiskUsageManager.js";
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
   jQuery.ajaxSetup({
      type: 'GET',
      dataType: 'jsonp',
      timeout: 3000,
      cache: false,
      global: false
   });

   edu.cmu.ri.terk.DiskUsageManager = function(host)
      {
      var eventListeners = new Array();
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

      this.getDiskUsage = function()
         {
         notifyEventListeners('onBeforeLoad');

         // load the config
         jQuery.ajax(
         {
            url: host + '/cgi-bin/getDiskUsageAsJSON.pl',
            success: function(jsonResponse)
               {
               json = jsonResponse;

               if (json)
                  {
                  notifyEventListeners('onLoadSuccess');
                  }
               else
                  {
                  notifyEventListeners('onLoadFailure');
                  }
               },
            error: function()
               {
               notifyEventListeners('onLoadFailure');
               json = null;
               }
         });
         };

      /**
       * Registers the given event listener.  The event listener should implement some or all of the following methods:
       *
       *    onBeforeLoad()
       *    onLoadSuccess()
       *    onLoadFailure()
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

      this.filesystemIterator = function(callbackFunction)
         {
         if (callbackFunction)
            {
            var filesystems = getJSONProperty(json, "disk-usage.filesystems");
            jQuery.each(filesystems, function(i, filesystemJSON)
               {
               var filesystemJSONCopy = createCopyOfFileSystemJSON(filesystemJSON);
               if (filesystemJSONCopy)
                  {
                  callbackFunction(filesystemJSONCopy);
                  }
               });
            }
         };

      var createCopyOfFileSystemJSON = function(originalFilesystemJSON)
         {
         if (originalFilesystemJSON)
            {
            var newFilesystemJSON = {
               "filesystem" : originalFilesystemJSON['filesystem'],
               "total" : originalFilesystemJSON['total'],
               "used" : originalFilesystemJSON['used'],
               "available" : originalFilesystemJSON['available'],
               "use-percentage" : originalFilesystemJSON['use-percentage'],
               "mounted-on" : originalFilesystemJSON['mounted-on']
            };

            return newFilesystemJSON;
            }
         return null;
         };

      this.getBlockSizeUnits = function()
         {
         return getJSONProperty(json, "disk-usage.units");
         };
      // ---------------------------------------------------------------------------------------------------------------
      };

   // ==================================================================================================================
   })();
