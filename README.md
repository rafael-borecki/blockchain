- Para alterar a dificuldade, editar a HASH META em src/HashUtils.h, quanto mais zeros mais difícil.  
- Para aumentar o tempo entre a chegada de dados do data source simulado, editar o sleep no src/DataProducer.cpp.  
- A blockchain fica salva no path passado em FILENAME.  
- Para carregar uma blockchain existente, basta passar a filename de um arquivo já existente.  

Compilar:  
    ```make all```  

Executar:  
    ```./blockchain <FILENAME> <THREADS> <CHAIN HEIGHT>```  

![](./blockchain-uml.png)  

TODO: 
1. Transformar a HashMeta em uma variável para executar o programa assim:  
   ./blockchain <filename> <threads> <height> <difficulty>  
   Sendo difficulty um inteiro representando a quantidade de 0 na Hash Meta.  
2. Fazer com que não haja possibilidade de o Nonce esgotar  
3. Salvar o arquivo binário realmente em modo binário  
4. Conexão com Apache Kafka  

Com muita coragem:  
5. Uma UI    
6. P2P   
7. Merkle Tree  
8. Mecanismo de recomensa  

      
