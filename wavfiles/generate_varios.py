import numpy as np
import wave

# Parameters for WAV file
sample_rate = 44100  # 44.1 kHz sample rate
duration = 10  # 10 seconds of audio
frequencies = [440, 880, 1200]  # Multiple frequencies for variation
num_samples = sample_rate * duration

# Generate sine waves with varying amplitudes and frequencies
time_values = np.linspace(0, duration, num_samples)

# Create signal by adding multiple sine waves with different frequencies and amplitudes
signal = 0.5 * np.sin(2 * np.pi * frequencies[0] * time_values)  # Moderate amplitude
signal += 0.3 * np.sin(2 * np.pi * frequencies[1] * time_values)  # Lower amplitude
signal += 0.1 * np.sin(2 * np.pi * frequencies[2] * time_values)  # Very low amplitude

# Normalize the signal to the range [-32767, 32767] for 16-bit WAV format
signal_int16 = np.int16(signal / np.max(np.abs(signal)) * 32767)

# Write to WAV file
wav_filename = "/mnt/data/varied_audio_signal.wav"
with wave.open(wav_filename, 'w') as wav_file:
    wav_file.setnchannels(1)  # Mono
    wav_file.setsampwidth(2)  # 16-bit
    wav_file.setframerate(sample_rate)
    wav_file.writeframes(signal_int16.tobytes())

wav_filename  # Output the file path
