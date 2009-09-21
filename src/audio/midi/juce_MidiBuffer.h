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

#ifndef __JUCE_MIDIBUFFER_JUCEHEADER__
#define __JUCE_MIDIBUFFER_JUCEHEADER__

#include "../../containers/juce_ArrayAllocationBase.h"
#include "juce_MidiMessage.h"


//==============================================================================
/**
    Holds a sequence of time-stamped midi events.

    Analogous to the AudioSampleBuffer, this holds a set of midi events with
    integer time-stamps. The buffer is kept sorted in order of the time-stamps.

    @see MidiMessage
*/
class JUCE_API  MidiBuffer  : private ArrayAllocationBase <uint8>
{
public:
    //==============================================================================
    /** Creates an empty MidiBuffer. */
    MidiBuffer() throw();

    /** Creates a MidiBuffer containing a single midi message. */
    MidiBuffer (const MidiMessage& message) throw();

    /** Creates a copy of another MidiBuffer. */
    MidiBuffer (const MidiBuffer& other) throw();

    /** Makes a copy of another MidiBuffer. */
    const MidiBuffer& operator= (const MidiBuffer& other) throw();

    /** Destructor */
    ~MidiBuffer() throw();

    //==============================================================================
    /** Removes all events from the buffer. */
    void clear() throw();

    /** Removes all events between two times from the buffer.

        All events for which (start <= event position < start + numSamples) will
        be removed.
    */
    void clear (const int start,
                const int numSamples) throw();

    /** Returns true if the buffer is empty.

        To actually retrieve the events, use a MidiBuffer::Iterator object
    */
    bool isEmpty() const throw();

    /** Counts the number of events in the buffer.

        This is actually quite a slow operation, as it has to iterate through all
        the events, so you might prefer to call isEmpty() if that's all you need
        to know.
    */
    int getNumEvents() const throw();

    /** Adds an event to the buffer.

        The sample number will be used to determine the position of the event in
        the buffer, which is always kept sorted. The MidiMessage's timestamp is
        ignored.

        If an event is added whose sample position is the same as one or more events
        already in the buffer, the new event will be placed after the existing ones.

        To retrieve events, use a MidiBuffer::Iterator object
    */
    void addEvent (const MidiMessage& midiMessage,
                   const int sampleNumber) throw();

    /** Adds an event to the buffer from raw midi data.

        The sample number will be used to determine the position of the event in
        the buffer, which is always kept sorted.

        If an event is added whose sample position is the same as one or more events
        already in the buffer, the new event will be placed after the existing ones.

        The event data will be inspected to calculate the number of bytes in length that
        the midi event really takes up, so maxBytesOfMidiData may be longer than the data
        that actually gets stored. E.g. if you pass in a note-on and a length of 4 bytes,
        it'll actually only store 3 bytes. If the midi data is invalid, it might not
        add an event at all.

        To retrieve events, use a MidiBuffer::Iterator object
    */
    void addEvent (const uint8* const rawMidiData,
                   const int maxBytesOfMidiData,
                   const int sampleNumber) throw();

    /** Adds some events from another buffer to this one.

        @param otherBuffer          the buffer containing the events you want to add
        @param startSample          the lowest sample number in the source buffer for which
                                    events should be added. Any source events whose timestamp is
                                    less than this will be ignored
        @param numSamples           the valid range of samples from the source buffer for which
                                    events should be added - i.e. events in the source buffer whose
                                    timestamp is greater than or equal to (startSample + numSamples)
                                    will be ignored. If this value is less than 0, all events after
                                    startSample will be taken.
        @param sampleDeltaToAdd     a value which will be added to the source timestamps of the events
                                    that are added to this buffer
    */
    void addEvents (const MidiBuffer& otherBuffer,
                    const int startSample,
                    const int numSamples,
                    const int sampleDeltaToAdd) throw();

    /** Returns the sample number of the first event in the buffer.

        If the buffer's empty, this will just return 0.
    */
    int getFirstEventTime() const throw();

    /** Returns the sample number of the last event in the buffer.

        If the buffer's empty, this will just return 0.
    */
    int getLastEventTime() const throw();

    //==============================================================================
    /** Exchanges the contents of this buffer with another one.

        This is a quick operation, because no memory allocating or copying is done, it
        just swaps the internal state of the two buffers.
    */
    void swap (MidiBuffer& other);

    //==============================================================================
    /**
        Used to iterate through the events in a MidiBuffer.

        Note that altering the buffer while an iterator is using it isn't a
        safe operation.

        @see MidiBuffer
    */
    class Iterator
    {
    public:
        //==============================================================================
        /** Creates an Iterator for this MidiBuffer. */
        Iterator (const MidiBuffer& buffer) throw();

        /** Destructor. */
        ~Iterator() throw();

        //==============================================================================
        /** Repositions the iterator so that the next event retrieved will be the first
            one whose sample position is at greater than or equal to the given position.
        */
        void setNextSamplePosition (const int samplePosition) throw();

        /** Retrieves a copy of the next event from the buffer.

            @param result   on return, this will be the message (the MidiMessage's timestamp
                            is not set)
            @param samplePosition   on return, this will be the position of the event
            @returns        true if an event was found, or false if the iterator has reached
                            the end of the buffer
        */
        bool getNextEvent (MidiMessage& result,
                           int& samplePosition) throw();

        /** Retrieves the next event from the buffer.

            @param midiData     on return, this pointer will be set to a block of data containing
                                the midi message. Note that to make it fast, this is a pointer
                                directly into the MidiBuffer's internal data, so is only valid
                                temporarily until the MidiBuffer is altered.
            @param numBytesOfMidiData   on return, this is the number of bytes of data used by the
                                        midi message
            @param samplePosition   on return, this will be the position of the event
            @returns        true if an event was found, or false if the iterator has reached
                            the end of the buffer
        */
        bool getNextEvent (const uint8* &midiData,
                           int& numBytesOfMidiData,
                           int& samplePosition) throw();

        //==============================================================================
        juce_UseDebuggingNewOperator

    private:
        const MidiBuffer& buffer;
        const uint8* data;

        Iterator (const Iterator&);
        const Iterator& operator= (const Iterator&);
    };


    //==============================================================================
    juce_UseDebuggingNewOperator

private:
    friend class MidiBuffer::Iterator;
    int bytesUsed;

    uint8* findEventAfter (uint8* d, const int samplePosition) const throw();
};


#endif   // __JUCE_MIDIBUFFER_JUCEHEADER__