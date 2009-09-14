//======================================================================================================================
// Class for initializing and configuring the various widgets on the System Info tab.
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
   var noJQuery = "The jQuery library is required by edu.cmu.ri.terk.SystemInfoTab.js";
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

   edu.cmu.ri.terk.SystemInfoTab = function(versionInfoManager, memoryUsageManager, diskUsageManager, powerInfoManager)
      {
      var isVersionInfoLoaded = false;

      // register the VersionInfoManager event listener
      versionInfoManager.addEventListener({
         onBeforeLoad : function()
            {
            jQuery("#versionInfoMessageArea").html("Loading system information...");
            jQuery("#versionInfoArea").hide();
            },
         onLoadSuccess : function()
            {
            isVersionInfoLoaded = true;
            jQuery("#versionInfoHardwareVersion").text(versionInfoManager.getHardwareVersion());

               var firmwareVersion = versionInfoManager.getFirmwareVersion();
            jQuery("#versionInfoFirmwareVersion").text(firmwareVersion['major'] +
                                                       "." +
                                                       firmwareVersion['minor'] +
                                                       "." +
                                                       firmwareVersion['revision'] +
                                                       " (" +
                                                       versionInfoManager.getFirmwareTimestamp() +
                                                       ")");

            var osInfo = versionInfoManager.getOperatingSystemInfo();
            jQuery("#versionInfoOperatingSystem").text(osInfo['name'] + " " + osInfo['release'] + " (" + osInfo['version'] + " " + osInfo['machine-type'] + " " + osInfo['processor-type'] + ")");

            jQuery("#versionInfoMessageArea").empty();
            jQuery("#versionInfoArea").show();
            },
         onLoadFailure : function()
            {
            jQuery("#versionInfoMessageArea").html("Sorry, the system information is currently unavailable.");
            }});

      // register the PowerInfoManager event listener
      powerInfoManager.addEventListener({
         onBeforeLoad : function()
            {
            jQuery("#powerInfoMessageArea").html("Loading power information...");
            jQuery("#powerInfoArea").hide();
            },
         onLoadSuccess : function()
            {
            jQuery("#powerInfoMainBatteryVoltage").text(powerInfoManager.getMainBatteryVoltage());
            jQuery("#powerInfoMainBatteryState").text(powerInfoManager.getMainBatteryState());
            jQuery("#powerInfoBackupBatteryVoltage").text(powerInfoManager.getBackupBatteryVoltage());
            jQuery("#powerInfoBackupBatteryState").text(powerInfoManager.getBackupBatteryState());
            jQuery("#powerInfoIo5VVoltage").text(powerInfoManager.getIO5VVoltage());
            jQuery("#powerInfoIo5VState").text(powerInfoManager.getIO5VState());
            jQuery("#powerInfoMain5VVoltage").text(powerInfoManager.getMain5VVoltage());

            // remove any dynamically-generated cells created from previous visits to this page
            jQuery("#powerInfoBreakersTable .dynamically-generated").remove();

            // generate the data cells
            var breakers = powerInfoManager.getBreakersState();
            jQuery.each(breakers, function(i, breakerState)
               {
               var value = breakerState ? '<span class="status-fault">Fault</span>' : '<span class="status-ok">OK</span>';
               var labelCell = jQuery('<td class="data-column-label dynamically-generated">' + (i + 1) + '</td>');
               jQuery("#powerInfoBreakersTableHeadingRow").append(labelCell);
               var dataCell = jQuery('<td class="data-cell dynamically-generated">' + value + '</td>');
               jQuery("#powerInfoBreakersTableDataRow").append(dataCell);
               });

            jQuery("#powerInfoMessageArea").empty();
            jQuery("#powerInfoArea").show();
            },
         onLoadFailure : function()
            {
            jQuery("#powerInfoMessageArea").html("Sorry, the power information is currently unavailable.");
            },
         onLoadComplete : function()
            {
            // Only load the version info once since it won't change.  See the note in the activate() method which
            // explains why I'm loading version info here instead.
            if (!isVersionInfoLoaded)
               {
               versionInfoManager.getVersionInfo();
               }
            }});

      // register the MemoryUsageManager event listener
      memoryUsageManager.addEventListener({
         onBeforeLoad : function()
            {
            jQuery("#memoryUsageMessageArea").html("Loading memory usage...");
            jQuery("#memoryUsageArea").hide();
            },
         onLoadSuccess : function()
            {
            var mainMemoryUsage = memoryUsageManager.getMainMemoryUsage();
            if (mainMemoryUsage)
               {
               jQuery("#memoryUsageMemoryTotal").text(mainMemoryUsage['total']);
               jQuery("#memoryUsageMemoryUsed").text(mainMemoryUsage['used']);
               jQuery("#memoryUsageMemoryFree").text(mainMemoryUsage['free']);
               }

            var swapMemoryUsage = memoryUsageManager.getSwapMemoryUsage();
            if (swapMemoryUsage)
               {
               jQuery("#memoryUsageSwapTotal").text(swapMemoryUsage['total']);
               jQuery("#memoryUsageSwapUsed").text(swapMemoryUsage['used']);
               jQuery("#memoryUsageSwapFree").text(swapMemoryUsage['free']);
               }

            var totalMemoryUsage = memoryUsageManager.getTotalMemoryUsage();
            if (totalMemoryUsage)
               {
               jQuery("#memoryUsageTotalTotal").text(totalMemoryUsage['total']);
               jQuery("#memoryUsageTotalUsed").text(totalMemoryUsage['used']);
               jQuery("#memoryUsageTotalFree").text(totalMemoryUsage['free']);
               }

            jQuery("#memoryUsageMessageArea").empty();
            jQuery("#memoryUsageArea").show();
            },
         onLoadFailure : function()
            {
            jQuery("#memoryUsageMessageArea").html("Sorry, the memory usage information is currently unavailable.");
            }});

      // register the DiskUsageManager event listener
      diskUsageManager.addEventListener({
         onBeforeLoad : function()
            {
            jQuery("#diskUsageMessageArea").html("Loading disk usage...");
            jQuery("#diskUsageArea").hide();
            },
         onLoadSuccess : function()
            {
            jQuery("#diskUsageBlockSizeUnits").text(diskUsageManager.getBlockSizeUnits());

            // remove any dynamically-generated cells created from previous visits to this page
            jQuery("#diskUsageTable .dynamically-generated").remove();

            // generate the data cells
            diskUsageManager.filesystemIterator(function(filesystemJSON)
               {
               jQuery("#diskUsageTable").append('<tr class="dynamically-generated">' +
                                                '<td class="data-cell align-left">' + filesystemJSON['filesystem'] + '</td>' +
                                                '<td class="data-cell align-right">' + filesystemJSON['total'] + '</td>' +
                                                '<td class="data-cell align-right">' + filesystemJSON['used'] + '</td>' +
                                                '<td class="data-cell align-right">' + filesystemJSON['available'] + '</td>' +
                                                '<td class="data-cell align-right">' + filesystemJSON['use-percentage'] + '</td>' +
                                                '<td class="data-cell align-left">' + filesystemJSON['mounted-on'] + '</td>' +
                                                '</tr>');
               });

            jQuery("#diskUsageMessageArea").empty();
            jQuery("#diskUsageArea").show();
            },
         onLoadFailure : function()
            {
            jQuery("#diskUsageMessageArea").html("Sorry, the disk usage information is currently unavailable.");
            }});

      this.activate = function()
         {
         // Note that we don't call versionInfoManager.getVersionInfo() here due to a race condition: both the
         // PowerInfoManager and VersionInfoManager ultimately depend on the CQEPower class in libqwerk (because
         // the hardware version number is kept in CQEPower) so if they both try to fetch the status at the same time,
         // sometimes one will fail if the other has a lock on CQEPower.  I don't love solving this in the front end,
         // but it'll do for now.  The simple solution is to call versionInfoManager.getVersionInfo() AFTER the
         // call to powerInfoManager.getPowerInfo() is complete.
         powerInfoManager.getPowerInfo();
         memoryUsageManager.getMemoryUsage();
         diskUsageManager.getDiskUsage();
         };
      // ---------------------------------------------------------------------------------------------------------------
      };

   // ==================================================================================================================
   })();
