/*
  ==============================================================================

   This file is part of the JUCE library - "Jules' Utility Class Extensions"
   Copyright 2004-11 by Raw Material Software Ltd.

  ------------------------------------------------------------------------------

   JUCE can be redistributed and/or modified under the terms of the GNU General
   Public License (Version 2), as published by the Free Software Foundation.
   A copy of the license is included in the JUCE distribution, or can be found
   online at www.gnu.org/licenses.

   JUCE is distributed in the hope that it will be useful, but WITHOUT ANY
   WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
   A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

  ------------------------------------------------------------------------------

   To release a closed-source product which uses JUCE, commercial licenses are
   available: visit www.rawmaterialsoftware.com/juce for more information.

  ==============================================================================
*/

#ifndef __JUCE_WINDOWSREGISTRY_JUCEHEADER__
#define __JUCE_WINDOWSREGISTRY_JUCEHEADER__

#if JUCE_WINDOWS || DOXYGEN

/**
    Contains some static helper functions for manipulating the MS Windows registry
    (Only available on Windows, of course!)
*/
class WindowsRegistry
{
    //==============================================================================
    /** Returns a string from the registry.

        The path is a string for the entire path of a value in the registry,
        e.g. "HKEY_CURRENT_USER\Software\foo\bar"
    */
    static String getValue (const String& regValuePath,
                            const String& defaultValue = String::empty);

    /** Sets a registry value as a string.

        This will take care of creating any groups needed to get to the given
        registry value.
    */
    static void setValue (const String& regValuePath,
                          const String& value);

    /** Returns true if the given value exists in the registry. */
    static bool valueExists (const String& regValuePath);

    /** Deletes a registry value. */
    static void deleteValue (const String& regValuePath);

    /** Deletes a registry key (which is registry-talk for 'folder'). */
    static void deleteKey (const String& regKeyPath);

    /** Creates a file association in the registry.

        This lets you set the executable that should be launched by a given file extension.
        @param fileExtension        the file extension to associate, including the
                                    initial dot, e.g. ".txt"
        @param symbolicDescription  a space-free short token to identify the file type
        @param fullDescription      a human-readable description of the file type
        @param targetExecutable     the executable that should be launched
        @param iconResourceNumber   the icon that gets displayed for the file type will be
                                    found by looking up this resource number in the
                                    executable. Pass 0 here to not use an icon
    */
    static void registerFileAssociation (const String& fileExtension,
                                         const String& symbolicDescription,
                                         const String& fullDescription,
                                         const File& targetExecutable,
                                         int iconResourceNumber);

private:
    WindowsRegistry();
    JUCE_DECLARE_NON_COPYABLE (WindowsRegistry);
};

#endif
#endif   // __JUCE_WINDOWSREGISTRY_JUCEHEADER__