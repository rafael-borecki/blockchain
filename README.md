- Para alterar a dificuldade, editar a HASH META em src/HashUtils.h, quanto mais zeros mais difícil.  
- Para aumentar o tempo entre a chegada de dados do data source simulado, editar o sleep no src/DataProducer.cpp.  
- A blockchain fica salva no path passado em FILENAME.  
- Para carregar uma blockchain existente, basta passar a filename de um arquivo já existente.  

Compilar:  
    ```make all```  

Executar:  
    ```./blockchain <FILENAME> <THREADS> <CHAIN HEIGHT>```  

Executar com Apache Kafka e Docker:
```bash
docker compose up --build -d 
```

![](./blockchain-uml.png)  
![](./blockchain-architecture.png)  

      
