//======================================================================================================================
// Class for obtaining the wireless networking status.
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
   var nojQueryMsg = "The jQuery library is required by edu.cmu.ri.terk.WirelessNetworkingStatus.js";
   alert(nojQueryMsg);
   throw new Error(nojQueryMsg);
   }
//======================================================================================================================

//======================================================================================================================
// CODE
//======================================================================================================================
(function()
   {
   var jQuery = window['$'];
   jQuery.ajaxSetup({
      type: 'GET',
      dataType: 'jsonp',
      timeout: 3000,
      cache: false,
      global: false
   });

   edu.cmu.ri.terk.WirelessNetworkingStatus = function(wirelessNetworkingStatusMessageAreaId,
                                                       wirelessNetworkingStatusMessageAreaDetailId,
                                                       host)
      {
      this.getWirelessNetworkingStatus = function()
         {
         displayStatus("&nbsp;", "Checking status...");

         var wirelessNetworkingStatus = null;

         // load the wireless status
         jQuery.ajax(
         {
            url: host + '/cgi-bin/getWirelessNetworkingStatusAsJSON.pl',
            success: function(jsonResponse)
               {
               if (jsonResponse && jsonResponse['wireless-networking-status'])
                  {
                  wirelessNetworkingStatus = jsonResponse['wireless-networking-status'];
                  }
               displayWirelessNetworkingStatus(wirelessNetworkingStatus);
               },
            error: function()
               {
               displayWirelessNetworkingStatus(wirelessNetworkingStatus);
               }
         });
         };

      var displayWirelessNetworkingStatus = function(wirelessNetworkingStatusJSON)
         {
         if (wirelessNetworkingStatusJSON)
            {
            if (wirelessNetworkingStatusJSON["is-installed"])
               {
               var wirelessInterface = wirelessNetworkingStatusJSON['wireless-interface'];
               if (wirelessInterface)
                  {
                  if (wirelessInterface['is-enabled'])
                     {
                     displayStatus("Connected", "The wireless adapter is connected to " + wirelessInterface['access-point']['ssid'] + " and has the IP address " + wirelessInterface['access-point']['ip-address']);
                     }
                  else
                     {
                     displayStatus("Off", "The wireless interface is disabled.");
                     }
                  }
               else
                  {
                  displayStatus("Unknown", "Failed to retrieve status.");
                  }
               }
            else
               {
               displayStatus("Off", "The wireless adapter is unplugged.");
               }
            }
         else
            {
            displayStatus("Unknown", "Failed to retrieve status.");
            }
         };

      var displayStatus = function(message, detail)
         {
         jQuery("#" + wirelessNetworkingStatusMessageAreaId).html(message);
         jQuery("#" + wirelessNetworkingStatusMessageAreaDetailId).html(detail);
         };

      };
   // ==================================================================================================================
   })();

