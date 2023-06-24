import random
import string
import struct

# Função para gerar um nome aleatório
def generate_random_name(length):
    letters = string.ascii_lowercase
    return ''.join(random.choice(letters) for _ in range(length))

# Gerar 400 registros aleatórios
registros = []
for i in range(400):
    seq_curso = i + 1
    codigo_curso = random.choice(['C01', 'C02', 'C03'])
    nome_curso = generate_random_name(10)
    registros.append((seq_curso, codigo_curso, nome_curso))

# Salvar os registros no arquivo curso.dat em formato binário
with open('curso.dat', 'wb') as file:
    for registro in registros:
        seq_curso, codigo_curso, nome_curso = registro
        data = struct.pack('i3s10s', seq_curso, codigo_curso.encode(), nome_curso.encode())
        file.write(data)
