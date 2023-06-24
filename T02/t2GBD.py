import os
import struct

class Registro:
    def __init__(self, nseq, text):
        self.nseq = nseq
        self.text = text
    def __str__(self):
    	return f"{self.nseq} - {self.text}"

class Pagina:
    def __init__(self, tamanho_max):
        self.registros = []
        self.tamanho_max = tamanho_max

    def adicionar_registro(self, registro):
        if len(self.registros) < self.tamanho_max:
            self.registros.append(registro)
            return True
        return False

    def remover_registro(self, nseq):
        for registro in self.registros:
            if registro.nseq == nseq:
                self.registros.remove(registro)
                return True
        return False

    def buscar_registro(self, nseq):
        for registro in self.registros:
            if registro.nseq == nseq:
                return registro
        return None

class HashExpansivo:
    def __init__(self, tamanho_inicial=10, tamanho_pagina=5):
        self.tamanho = tamanho_inicial
        self.tamanho_pagina = tamanho_pagina
        self.arquivo = "hash_file.bin"
        self.buckets = [None] * self.tamanho

    def gerar_chave(self, nseq):
        return nseq % self.tamanho

    def adicionar_elemento(self, registro):
        chave = self.gerar_chave(registro.nseq)
        if self.buckets[chave] is None:
            self.buckets[chave] = Pagina(self.tamanho_pagina)
        pagina = self.buckets[chave]
        if pagina.adicionar_registro(registro):
            with open(self.arquivo, "ab") as file:
                registro_data = struct.pack("i96s", registro.nseq, registro.text.encode())
                file.write(registro_data)
                return True
        return False

    def buscar_elemento(self, nseq):
        chave = self.gerar_chave(nseq)
        if self.buckets[chave] is None:
            return None
        pagina = self.buckets[chave]
        registro = pagina.buscar_registro(nseq)
        if registro is not None:
            return registro.text
        return None

    def remover_elemento(self, nseq):
        chave = self.gerar_chave(nseq)
        if self.buckets[chave] is None:
            return False
        pagina = self.buckets[chave]
        if pagina.remover_registro(nseq):
            with open(self.arquivo, "rb+") as file:
                registro_size = struct.calcsize("i96s")
                while True:
                    posicao_atual = file.tell()
                    registro_data = file.read(registro_size)
                    if not registro_data:
                        break
                    nseq_read, text_read = struct.unpack("i96s", registro_data)
                    if nseq_read == nseq:
                        file.seek(posicao_atual)
                        file.write(b'\x00' * registro_size)
                        return True
        return False

    def redimensionar_hash(self):
        self.tamanho *= 2
        novo_buckets = [None] * self.tamanho

        with open(self.arquivo, "rb") as file:
            registro_size = struct.calcsize("i96s")
            while True:
                registro_data = file.read(registro_size)
                if not registro_data:
                    break
                nseq_read, text_read = struct.unpack("i96s", registro_data)
                registro = Registro(nseq_read, text_read.decode().rstrip())
                nova_chave = self.gerar_chave(registro.nseq)

                if novo_buckets[nova_chave] is None:
                    novo_buckets[nova_chave] = Pagina(self.tamanho_pagina)
                pagina = novo_buckets[nova_chave]
                pagina.adicionar_registro(registro)

        self.buckets = novo_buckets

    def mostrar_tabela(self):
        print("Tabela Hash:")
        for i, bucket in enumerate(self.buckets):
            print(f"Chave {i}:", bucket.registros if bucket is not None else None)

# Exemplo de uso
hash_expansivo = HashExpansivo()
registros = []

# Adicionar elementos
for i in range(80):
	registros.append(Registro(i, "Texto "+str(i)))

for i in range(80):
	hash_expansivo.adicionar_elemento(registros[i])


# Buscar elementos
print(hash_expansivo.buscar_elemento(2))  
print(hash_expansivo.buscar_elemento(4))  
print(hash_expansivo.buscar_elemento(30))  


print(hash_expansivo.remover_elemento(2))

# Buscar novamente
print(hash_expansivo.buscar_elemento(2)) 


# Mostrar tabela
hash_expansivo.mostrar_tabela()

