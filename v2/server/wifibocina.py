from flask import Flask, request
import pygame
import time
import os

app = Flask(__name__)

# Inicializa PyGame para la reproducción de sonido
pygame.init()

# Obtener la ruta del directorio del script
script_dir = os.path.dirname(__file__)

# Ruta a los archivos de audio
audio_dir = os.path.join(script_dir, 'audio')

# Cargar los sonidos correspondientes a las notas
sounds = {
    'C4': pygame.mixer.Sound(os.path.join(audio_dir, 'C4.wav')),
    'D4': pygame.mixer.Sound(os.path.join(audio_dir, 'D4.wav')),
    'E4': pygame.mixer.Sound(os.path.join(audio_dir, 'E4.wav')),
    'F4': pygame.mixer.Sound(os.path.join(audio_dir, 'F4.wav')),
    'G4': pygame.mixer.Sound(os.path.join(audio_dir, 'G4.wav')),
    'A4': pygame.mixer.Sound(os.path.join(audio_dir, 'A4.wav')),
    'B4': pygame.mixer.Sound(os.path.join(audio_dir, 'B4.wav'))
}

# Almacenar la reproducción actual
current_sounds = {}

@app.route('/', methods=['POST'])
def receive_note():
    note = request.data.decode('utf-8').strip()
    print(f"Recibida nota: {note}")

    if note in sounds:
        # Detener el sonido actual si existe
        if note in current_sounds:
            current_sounds[note].stop()

        # Reproducir el nuevo sonido
        sound = sounds[note]
        sound.play(-1)  # Reproducir en bucle
        current_sounds[note] = sound
    else:
        print(f"Nota no reconocida: {note}")

    return 'Nota recibida', 200

@app.route('/stop', methods=['POST'])
def stop_all_notes():
    print("Deteniendo todos los sonidos.")
    for sound in current_sounds.values():
        sound.stop()
    current_sounds.clear()
    return 'Todos los sonidos detenidos', 200

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=8080)
