#!/usr/bin/python

import json
import sys


inputFile = open(sys.argv[1], "r")
outputfile = open(sys.argv[2], "w")
variableNames = []

try:
    pluginMap = json.loads(inputFile.read())
    
except ValueError as e:
    print " --> Error loading input file \"%s\"" % (sys.argv[1])
    print "     %s" % (e)
    exit -1


def out(line):
    outputfile.write("%s\n" % line)

def extractVendors(pluginMap):
    for vendor in pluginMap['vendors']:
        try:
            out("VendorId %sVendorId = VendorId(\"%s\");" % pluginMap["idName"], pluginMap["id"])
        except:
            pass
        extractDeviceClasses(vendor)


def extractDeviceClasses(vendorMap):
    for deviceClass in vendorMap["deviceClasses"]:
        print("have deviceclass %s" % deviceClass["deviceClassId"])
        try:
            variableName = "%sDeviceClassId" % (deviceClass["idName"]) 
            if not variableName in variableNames:
                variableNames.append(variableName)
                out("DeviceClassId %s = DeviceClassId(\"%s\");" % (variableName, deviceClass["deviceClassId"]))
            else:
                print("duplicated variable name \"%s\" for DeviceClassId %s -> skipping") % (variableName, deviceClass["deviceClassId"])
        except:
            pass
        extractActionTypes(deviceClass)
        extractStateTypes(deviceClass)
        extractEventTypes(deviceClass)


def extractStateTypes(deviceClassMap):
    try:
        for stateType in deviceClassMap["stateTypes"]:
            try:
                variableName = "%sStateTypeId" % (stateType["idName"])
                if not variableName in variableNames:
                    variableNames.append(variableName)
                    out("StateTypeId %s = StateTypeId(\"%s\");" % (variableName, stateType["id"]))
                else:
                    print("duplicated variable name \"%s\" for StateTypeId %s -> skipping") % (variableName, stateType["id"])
                # create ActionTypeId if the state is writable
                if 'writable' in stateType:
                    print("create ActionTypeId for StateType %s" % stateType["id"])
                    vName = "%sActionTypeId" % (stateType["idName"]) 
                    if not vName in variableNames:
                        variableNames.append(vName)
                        out("ActionTypeId %s = ActionTypeId(\"%s\");" % (vName, stateType["id"]))
                    else:
                        print("duplicated variable name \"%s\" for ActionTypeId %s -> skipping") % (variableName, stateType["id"])
            except:
                pass
    except:
        pass


def extractActionTypes(deviceClassMap):
    try:
        for actionType in deviceClassMap["actionTypes"]:
            try:
                variableName = "%sActionTypeId" % (actionType["idName"])
                if not variableName in variableNames:
                    variableNames.append(variableName)
                    out("ActionTypeId %s = ActionTypeId(\"%s\");" % (variableName, actionType["id"]))
                else:
                    print("duplicated variable name \"%s\" for ActionTypeId %s -> skipping") % (variableName, actionType["id"])
            except:
                pass
    except:
        pass


def extractEventTypes(deviceClassMap):
    try:
        for eventType in deviceClassMap["eventTypes"]:
            try:
                variableName = "%sEventTypeId" % (eventType["idName"])
                if not variableName in variableNames:
                    variableNames.append(variableName)
                    out("EventTypeId %s = EventTypeId(\"%s\");" % (variableName, eventType["id"]))
                else:
                    print("duplicated variable name \"%s\" for EventTypeId %s -> skipping") % (variableName, eventType["id"])
            except:
                pass
    except:
        pass


print " --> generate plugininfo.h"
print "PluginId for plugin \"%s\" = %s" %(pluginMap['name'], pluginMap['id'])
    
# write header
out("/* This file is generated by the guh build system. Any changes to this file will")
out(" * be lost.")
out(" *")
out(" * If you want to change this file, edit the plugin's json file and add")
out(" * idName tags where appropriate.")
out(" */")

out("#ifndef PLUGININFO_H")
out("#define PLUGININFO_H")
out("#include \"typeutils.h\"")
out("")
out("PluginId pluginId = PluginId(\"%s\");" % pluginMap['id'])

extractVendors(pluginMap)

out("")
out("#endif")

print " --> finished writing \"%s\"" % (sys.argv[2])
