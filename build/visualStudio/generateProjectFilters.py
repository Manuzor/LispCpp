#/bin/env python3

import sys
import re
import os
import xml.etree.ElementTree as Xml

projFileNames = (
    'lcppCore.vcxproj',
    'lcppCLI.vcxproj',
    'lcppCore_unittests.vcxproj',
)

# Will be overwritten if the user specified anything
toStringRegexes = [r'..[/\\]..[/\\]source[/\\](lcppCore(_unittests)?|lcppCLI|lcppCore)[/\\]?']
clIncludesAndCompiles = []
filters = []

def isNotEmpty(string):
    return bool(string)

def parse(root):
    for child in root:
        if child.tag.endswith("ItemGroup"):
            for subChild in child:
                if subChild.tag.endswith("ClInclude") or subChild.tag.endswith("ClCompile"):
                    fileName = subChild.attrib["Include"]
                    clIncludesAndCompiles.append(fileName)

def basename(fileName):
    indexSlash = fileName.rfind('\\')
    if indexSlash >= 0:
        return fileName[:indexSlash]
    return None

def processLists():
    for content in clIncludesAndCompiles:
        content = basename(content.replace('/', '\\'))
        if not content:
            return

        for regex in toStringRegexes:
            content = re.sub(regex, '', content)

        if isNotEmpty(content) and content not in filters:
            print("New Filter Name: {content}".format(content=content))
            filters.append(content)

def generateFilterFileXml():
    root = Xml.Element("Project", {
        "ToolsVersion" : "4.0",
        "xmlns" : "http://schemas.microsoft.com/developer/msbuild/2003",
    })

    itemGroup = Xml.SubElement(root, "ItemGroup")

    for f in filters:
        element = Xml.SubElement(itemGroup, "Filter", { "Include" : f })

    return root

def appendToExistingFiltersFile(filterFileName, xml_filtersRoot, xml_existingRoot):
    print("Updating filters of existing file.")

def createNewFiltersFile(filterFileName, xml_filtersRoot):
    print("Creating new filters file.")

    xmlContent = """<?xml version="1.0" encoding="utf-8"?>\n"""
    xmlContent += Xml.tostring(xml_filtersRoot, encoding="unicode");

    with open(filterFileName, "w") as filterFile:
        filterFile.write(xmlContent)

def writeFiltersFile(xml_root, projFileName, filtersRoot, xml_root):
    filterFileName = "{0}.filters".format(projFileName)

    if os.path.isFile(filterFileName):
        appendToExistingFiltersFile(filterFileName, filterFileRoot, xml_root)
    else:
        createNewFiltersFile(filterFileName, filterFileRoot)

def main():
    if len(sys.argv) > 1:
        global toStringRegexes
        toStringRegexes = sys.argv[1:]
    print("toStringRegexes: {0}".format(toStringRegexes))
    for projFileName in projFileNames:
        global clIncludesAndCompiles
        global filters

        clIncludesAndCompiles = []
        filters = []

        root = Xml.parse(projFileName).getroot()
        parse(root)

        processLists()
        print("Filters: {0}".format(filters))

        filterFileRoot = generateFilterFileXml()
        writeFiltersFile(root, projFileName, filterFileRoot)

    #for f in filters:
    #    print(f)

if __name__ == '__main__':
    main()
