/*
  ==============================================================================

   This file is part of the JUCE library - "Jules' Utility Class Extensions"
   Copyright 2004-9 by Raw Material Software Ltd.

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

#ifndef __JUCE_AUDIOFORMAT_JUCEHEADER__
#define __JUCE_AUDIOFORMAT_JUCEHEADER__

#include "juce_AudioFormatReader.h"
#include "juce_AudioFormatWriter.h"


//==============================================================================
/**
    Subclasses of AudioFormat are used to read and write different audio
    file formats.

    @see AudioFormatReader, AudioFormatWriter, WavAudioFormat, AiffAudioFormat
*/
class JUCE_API  AudioFormat
{
public:
    //==============================================================================
    /** Destructor. */
    virtual ~AudioFormat();

    //==============================================================================
    /** Returns the name of this format.

        e.g. "WAV file" or "AIFF file"
    */
    const String& getFormatName() const;

    /** Returns all the file extensions that might apply to a file of this format.

        The first item will be the one that's preferred when creating a new file.

        So for a wav file this might just return ".wav"; for an AIFF file it might
        return two items, ".aif" and ".aiff"
    */
    const StringArray& getFileExtensions() const;

    //==============================================================================
    /** Returns true if this the given file can be read by this format.

        Subclasses shouldn't do too much work here, just check the extension or
        file type. The base class implementation just checks the file's extension
        against one of the ones that was registered in the constructor.
    */
    virtual bool canHandleFile (const File& fileToTest);

    /** Returns a set of sample rates that the format can read and write. */
    virtual const Array <int> getPossibleSampleRates() = 0;

    /** Returns a set of bit depths that the format can read and write. */
    virtual const Array <int> getPossibleBitDepths() = 0;

    /** Returns true if the format can do 2-channel audio. */
    virtual bool canDoStereo() = 0;

    /** Returns true if the format can do 1-channel audio. */
    virtual bool canDoMono() = 0;

    /** Returns true if the format uses compressed data. */
    virtual bool isCompressed();

    /** Returns a list of different qualities that can be used when writing.

        Non-compressed formats will just return an empty array, but for something
        like Ogg-Vorbis or MP3, it might return a list of bit-rates, etc.

        When calling createWriterFor(), an index from this array is passed in to
        tell the format which option is required.
    */
    virtual const StringArray getQualityOptions();

    //==============================================================================
    /** Tries to create an object that can read from a stream containing audio
        data in this format.

        The reader object that is returned can be used to read from the stream, and
        should then be deleted by the caller.

        @param sourceStream                 the stream to read from - the AudioFormatReader object
                                            that is returned will delete this stream when it no longer
                                            needs it.
        @param deleteStreamIfOpeningFails   if no reader can be created, this determines whether this method
                                            should delete the stream object that was passed-in. (If a valid
                                            reader is returned, it will always be in charge of deleting the
                                            stream, so this parameter is ignored)
        @see AudioFormatReader
    */
    virtual AudioFormatReader* createReaderFor (InputStream* sourceStream,
                                                const bool deleteStreamIfOpeningFails) = 0;

    /** Tries to create an object that can write to a stream with this audio format.

        The writer object that is returned can be used to write to the stream, and
        should then be deleted by the caller.

        If the stream can't be created for some reason (e.g. the parameters passed in
        here aren't suitable), this will return 0.

        @param streamToWriteTo      the stream that the data will go to - this will be
                                    deleted by the AudioFormatWriter object when it's no longer
                                    needed. If no AudioFormatWriter can be created by this method,
                                    the stream will NOT be deleted, so that the caller can re-use it
                                    to try to open a different format, etc
        @param sampleRateToUse      the sample rate for the file, which must be one of the ones
                                    returned by getPossibleSampleRates()
        @param numberOfChannels     the number of channels - this must be either 1 or 2, and
                                    the choice will depend on the results of canDoMono() and
                                    canDoStereo()
        @param bitsPerSample        the bits per sample to use - this must be one of the values
                                    returned by getPossibleBitDepths()
        @param metadataValues       a set of metadata values that the writer should try to write
                                    to the stream. Exactly what these are depends on the format,
                                    and the subclass doesn't actually have to do anything with
                                    them if it doesn't want to. Have a look at the specific format
                                    implementation classes to see possible values that can be
                                    used
        @param qualityOptionIndex   the index of one of compression qualities returned by the
                                    getQualityOptions() method. If there aren't any quality options
                                    for this format, just pass 0 in this parameter, as it'll be
                                    ignored
        @see AudioFormatWriter
    */
    virtual AudioFormatWriter* createWriterFor (OutputStream* streamToWriteTo,
                                                double sampleRateToUse,
                                                unsigned int numberOfChannels,
                                                int bitsPerSample,
                                                const StringPairArray& metadataValues,
                                                int qualityOptionIndex) = 0;

protected:
    /** Creates an AudioFormat object.

        @param formatName       this sets the value that will be returned by getFormatName()
        @param fileExtensions   a zero-terminated list of file extensions - this is what will
                                be returned by getFileExtension()
    */
    AudioFormat (const String& formatName,
                 const tchar** const fileExtensions);

private:
    //==============================================================================
    String formatName;
    StringArray fileExtensions;
};


#endif   // __JUCE_AUDIOFORMAT_JUCEHEADER__