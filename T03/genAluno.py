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
    seq_aluno = i + 1
    codigo_curso = random.choice(['C01', 'C02', 'C03'])
    nome_aluno = generate_random_name(10)
    registros.append((seq_aluno, codigo_curso, nome_aluno))

# Salvar os registros no arquivo aluno.dat em formato binário
with open('aluno.dat', 'wb') as file:
    for registro in registros:
        seq_aluno, codigo_curso, nome_aluno = registro
        data = struct.pack('i3s10s', seq_aluno, codigo_curso.encode(), nome_aluno.encode())
        file.write(data)

