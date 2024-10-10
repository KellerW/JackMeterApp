import numpy as np
from scipy.io.wavfile import write

# Parâmetros para o áudio
sample_rate = 44100  # 44.1kHz padrão
duration_per_note = 1  # Duração de cada nota em segundos
amplitude = 1  # Amplitude das notas (entre 0 e 1)

# Frequências das notas (em Hz) para a escala de C maior (C, D, E, F, G, A, B, C)
frequencies = [261.63, 293.66, 329.63, 349.23, 392.00, 440.00, 493.88, 523.25] 

# Criação de uma função para gerar uma onda senoidal
def generate_sine_wave(frequency, duration, sample_rate, amplitude):
    t = np.linspace(0, duration, int(sample_rate * duration), endpoint=False)
    return amplitude * np.sin(2 * np.pi * frequency * t)

# Gerar a sequência de notas
audio_data = np.concatenate([generate_sine_wave(freq, duration_per_note, sample_rate, amplitude) for freq in frequencies])

# Salvar o arquivo como um WAV
write('scale_c_major.wav', sample_rate, audio_data.astype(np.float32))

print("Arquivo de áudio 'scale_c_major.wav' gerado com sucesso.")
