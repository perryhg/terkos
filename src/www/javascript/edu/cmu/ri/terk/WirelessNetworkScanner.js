//======================================================================================================================
// Class for scanning for wireless networks.
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
   var nojQueryMsg = "The jQuery library is required by edu.cmu.ri.terk.WirelessNetworkScanner.js";
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

   edu.cmu.ri.terk.WirelessNetworkScanner = function(wirelessNetworksListId,
                                                     pleaseWaitIndicatorId)
      {
      var host = '';//http://192.168.1.104'; // TODO: remove me!

      var selectedWirelessNetwork = null;
      var selectionListeners = new Array();

      this.scanForWirelessNetworks = function()
         {
         jQuery("#" + pleaseWaitIndicatorId).removeClass("hidden");

         // clear the list
         jQuery("#" + wirelessNetworksListId).empty();

         // reset the selected network
         selectedWirelessNetwork = null;

         // notify listeners that nothing is selected
         jQuery.each(selectionListeners, function(i, listener)
            {
            if (listener)
               {
               listener(false);
               }
            });

         // load the network list
         jQuery.ajax(
         {
            url: host + '/cgi-bin/getWirelessNetworksAsJSON.pl?include-encrypted=false',
            success: function(wirelessNetworksJSON)
               {
               displayAvailableWirelessNetworks(wirelessNetworksJSON);
               },
            error: function()
               {
               displayAvailableWirelessNetworks(null);
               }
         });
         };

      var displayAvailableWirelessNetworks = function(wirelessNetworksJSON)
         {
         jQuery("#" + pleaseWaitIndicatorId).addClass("hidden");

         if (wirelessNetworksJSON && wirelessNetworksJSON['wireless-networks'])
            {
            var wirelessNetworks = wirelessNetworksJSON['wireless-networks'];
            if (wirelessNetworks && wirelessNetworks.length > 0)
               {
               for (var i = 0; i < wirelessNetworks.length; i++)
                  {
                  // build the list item
                  var listItem = document.createElement("div");
                  listItem.id = "availableWirelessNetworksListItem_" + i;

                  jQuery("#" + wirelessNetworksListId).append(listItem);
                  jQuery("#" + listItem.id).text(wirelessNetworks[i]['ssid']).disableSelection().addClass("list-item");

                  // add a mousedown event handler to the list item so we can keep
                  // track of which one is selected (and also do some selection highlighting)
                  jQuery("#" + listItem.id).mousedown(
                        function()
                           {
                           if (this != selectedWirelessNetwork)
                              {
                              if (selectedWirelessNetwork != null)
                                 {
                                 jQuery(selectedWirelessNetwork).removeClass("selected");
                                 }
                              jQuery(this).addClass("selected");
                              selectedWirelessNetwork = this;

                              // notify listeners that something is now selected
                              jQuery.each(selectionListeners, function(i, listener)
                                 {
                                 if (listener)
                                    {
                                    listener(true);
                                    }
                                 });
                              }
                           }
                        );
                  }
               }
            }
         else
            {
            // TODO
            }
         };

      this.addSelectionListener = function(listener)
         {
         if (listener)
            {
            selectionListeners[selectionListeners.length] = listener;
            }
         };

      this.isItemSelected = function()
         {
         return selectedWirelessNetwork != null;
         };
      };
   // ==================================================================================================================
   })();

