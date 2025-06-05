definitions:
    hash meta: HASH < 00000000000fffffffffffffffffffffffffffffffffffffffffffffffffffff
    node x: hash(previous block + nonce)
    nonce: hash(node id) + 4 bytes
    time-period: time allowed for each data sample to be processed, if time is due, accepts the first solution received.
    tie-break: smallest hash under the allowed time period
    attempts: each node can send only one valid nonce within the time period

    block N: 
    - block height  N
    - actual data   
    - timestamp 
    - hash meta 
    - hash            h (block N-1)
    - nonce           h (h(block N-1) + nonce)  < hash meta

architecture:
                                                .---------------------------------.
          .--> node 1 | nonce validator ---> valid ? append : drop                '
          '              ^                      ^               '                 '
          '              '                      '               v                 '
          '              '                      '---------wait for next nonce     '
          '              '                                                        '
          '              '                      .---------------------------------'
          .--> node 2 | nonce validator ---> valid ? append : drop                '
          '              '                      ^               '                 '
   block --              '                      '               v                 '-- new block  -- ...
   (data) '              '                      '---------wait for next nonce     '
          '              '                                                        '
          '              '                      .---------------------------------'
          '--> node 3 | nonce validator ---> valid ? append : drop                '
          '              ^                      ^               '                 '
          '              '                      '               v                 '
          '              '                      '---------wait for next nonce     '
          '--> node x - found valid nonce                                         '
          '              '                              .-------------------------'
          '              v                              '             
          '--> node N | nonce validator ---> valid ? append : drop    
                                                ^               '
                                                '               v
                                                '---------wait for next nonce
