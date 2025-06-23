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

TODO: 
- [ ] Transformar a HashMeta em uma variável para executar o programa assim:  
   ./blockchain <filename> <threads> <height> <difficulty>  
   Sendo difficulty um inteiro representando a quantidade de 0 na Hash Meta.  
- [ ] Fazer com que não haja possibilidade de o Nonce esgotar  
- [ ] Salvar o arquivo binário realmente em modo binário  
- [x] Conexão com Apache Kafka  

Com muita coragem:  
- [ ] Uma UI    
- [ ] P2P   
- [ ] Merkle Tree  
- [ ] Mecanismo de recomensa  

      
