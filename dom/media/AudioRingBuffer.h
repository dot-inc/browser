/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*-*/
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef MOZILLA_AUDIO_RING_BUFFER_H_
#define MOZILLA_AUDIO_RING_BUFFER_H_

#include "AudioSampleFormat.h"
#include "mozilla/Span.h"

#include <functional>

namespace mozilla {

/**
 * AudioRingBuffer works with audio sample format float or short. The
 * implementation wrap around the RingBuffer thus it is not thread-safe. Reads
 * and writes must happen in the same thread which may be different than the
 * construction thread. The memory is pre-allocated in the constructor. The
 * sample format has to be specified in order to be used.
 */
class AudioRingBuffer final {
 public:
  explicit AudioRingBuffer(int aSizeInBytes);
  ~AudioRingBuffer();

  /**
   * Set the sample format to either short or float. The sample format must be
   * set before the using any other method.
   */
  void SetSampleFormat(AudioSampleFormat aFormat);

  /**
   * Write `aBuffer.Length()` number of samples when the format is float.
   */
  int Write(const Span<const float>& aBuffer);

  /**
   * Write `aBuffer.Length()` number of samples when the format is short.
   */
  int Write(const Span<const int16_t>& aBuffer);

  /**
   * Write `aSamples` number of samples from `aBuffer`. Note the `aBuffer` does
   * not change.
   */
  int Write(const AudioRingBuffer& aBuffer, int aSamples);

  /**
   * Write `aSamples` number of zeros.
   */
  int WriteSilence(int aSamples);

  /**
   * Read `aBuffer.Length()` number of samples when the format is float.
   */
  int Read(const Span<float>& aBuffer);

  /**
   * Read `aBuffer.Length()` number of samples when the format is short.
   */
  int Read(const Span<int16_t>& aBuffer);

  /**
   * Read the internal buffer without extra copies when sample format is float.
   * Check also the RingBuffer::ReadNoCopy() for more details.
   */
  int ReadNoCopy(std::function<int(const Span<const float>&)>&& aCallable);

  /**
   * Read the internal buffer without extra copies when sample format is short.
   * Check also the RingBuffer::ReadNoCopy() for more details.
   */
  int ReadNoCopy(std::function<int(const Span<const int16_t>&)>&& aCallable);

  /**
   * Remove `aSamples` number of samples.
   */
  int Discard(int aSamples);

  /**
   * Remove all available samples.
   */
  int Clear();

  /**
   * Return true if the buffer is full.
   */
  bool IsFull() const;

  /**
   * Return true if the buffer is empty.
   */
  bool IsEmpty() const;

  /**
   * Return the number of samples available for writing.
   */
  int AvailableWrite() const;

  /**
   * Return the number of samples available for reading.
   */
  int AvailableRead() const;

 private:
  class AudioRingBufferPrivate;
  UniquePtr<AudioRingBufferPrivate> mPtr;
};

}  // namespace mozilla

#endif  // MOZILLA_AUDIO_RING_BUFFER_H_