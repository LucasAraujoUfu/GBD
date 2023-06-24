def merge_sort_join_file(aluno_file, curso_file, buffer_pool_size):
    PAGE_SIZE = 400  # Tamanho de uma página em bytes

    # Função para ler uma página de registros de um arquivo
    def read_page(file, page_num):
        file.seek(page_num * PAGE_SIZE)
        page_data = file.read(PAGE_SIZE)
        records = page_data.strip().split('\n')
        return records

    # Função para realizar a junção das páginas ordenadas
    def merge_pages(aluno_page, curso_page):
        result = []
        i = j = 0
        while i < len(aluno_page) and j < len(curso_page):
            aluno_data = aluno_page[i].split(',')
            curso_data = curso_page[j].split(',')
            codigo_aluno = aluno_data[1]
            codigo_curso = curso_data[1]

            if codigo_aluno == codigo_curso:
                # Realizar a combinação das linhas de aluno e curso
                row = {
                    'SEQ_ALUNO': int(aluno_data[0]),
                    'CODIGO_CURSO': codigo_aluno,
                    'NOME_ALUNO': aluno_data[2],
                    'NOME_CURSO': curso_data[2]
                }
                result.append(row)
                i += 1
                j += 1
            elif codigo_aluno < codigo_curso:
                i += 1
            else:
                j += 1

        return result

    # Abrir os arquivos de aluno e curso
    with open(aluno_file, 'rb') as aluno_f, open(curso_file, 'rb') as curso_f:
        # Definir o tamanho do buffer pool em número de páginas
        buffer_size = buffer_pool_size

        # Inicializar o buffer pool
        buffer_pool = [None] * buffer_size

        # Ler as primeiras páginas de cada arquivo
        aluno_page_num = 0
        curso_page_num = 0
        buffer_index = 0
        while aluno_page_num < buffer_size and curso_page_num < buffer_size:
            buffer_pool[buffer_index] = (
                read_page(aluno_f, aluno_page_num),
                read_page(curso_f, curso_page_num)
            )
            buffer_index += 1
            aluno_page_num += 1
            curso_page_num += 1

        # Realizar a junção das páginas
        result = []
        while True:
            # Encontrar a menor chave de curso no buffer pool
            min_curso_key = float('inf')
            min_curso_index = None
            for i, (aluno_page, curso_page) in enumerate(buffer_pool):
                if curso_page and curso_page[0].split(',')[1] < min_curso_key:
                    min_curso_key = curso_page[0].split(',')[1]
                    min_curso_index = i

            if min_curso_index is None:
                # Não há mais páginas de curso no buffer pool
                break

            # Encontrar as páginas de aluno correspondentes
            matching_aluno_pages = []
            for i, (aluno_page, _) in enumerate(buffer_pool):
                if aluno_page and aluno_page[0].split(',')[1] == min_curso_key:
                    matching_aluno_pages.append(i)

            if not matching_aluno_pages:
                # Não há páginas de aluno correspondentes
                break

            # Realizar a junção das páginas correspondentes
            for aluno_index in matching_aluno_pages:
                aluno_page, curso_page = buffer_pool[aluno_index]
                result.extend(merge_pages(aluno_page, curso_page))

                # Remover as páginas do buffer pool
                buffer_pool[aluno_index] = (None, None)

                # Ler a próxima página de aluno correspondente, se existir
                aluno_page_num += 1
                if aluno_page_num < buffer_size:
                    buffer_pool[aluno_index] = (
                        read_page(aluno_f, aluno_page_num),
                        curso_page
                    )

            # Atualizar as páginas de curso do buffer pool
            curso_page_num += 1
            if curso_page_num < buffer_size:
                buffer_pool[min_curso_index] = (
                    aluno_page,
                    read_page(curso_f, curso_page_num)
                )
            else:
                buffer_pool[min_curso_index] = (aluno_page, None)

    return result

# Exemplo de utilização
aluno_file = 'aluno.dat'
curso_file = 'curso.dat'
buffer_pool_size = 20  # Tamanho do buffer pool em número de páginas

result = merge_sort_join_file(aluno_file, curso_file, buffer_pool_size)
for row in result:
    print(row)
