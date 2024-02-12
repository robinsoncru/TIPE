une strategie est definie ainsi :
Strat = (e, h)
- h est nommee heuristique
    - h : S -> R    
- e est la fonction d'ecrasement
    - e : R -> R
    - e est continue (deux situations similaires donnent un score similaire)
        - lipschitzianité ?
    - e est strictement croissante (pour respecter les classements de qualité des situations)
    - e admet des limites finies en +inf et -inf
    - pour tout x dans R, xe(x) >= 0

