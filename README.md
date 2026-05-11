# DoomStickC MVP v1.7 - Audio and Feedback

Versão preparada para teste.

⚠️ README oficial do projeto deve ser atualizada somente depois da validação 100% no M5StickC Plus2.

## O que entrou para testar

- Som curto de início
- Som de tiro
- Som de munição vazia
- Som de dano
- Som de pickup
- Som de porta
- Som de vitória/morte
- Flash visual de pickup
- Flash visual de munição vazia
- Pulso visual de HP baixo
- Base v1.6 preservada

## Se o speaker causar problema

No `src/main.cpp`, troque:

```cpp
static constexpr bool AUDIO_ENABLED = true;
```

para:

```cpp
static constexpr bool AUDIO_ENABLED = false;
```

## Como instalar

```bash
pio run -t upload
```

## Checklist rápido

1. Compila sem erro.
2. Sobe para o M5StickC Plus2.
3. Tela inicial aparece.
4. A inicia o jogo.
5. Tiro emite feedback sonoro.
6. Munição vazia emite feedback e borda amarela.
7. Pickup emite som e flash verde.
8. Porta emite som.
9. Dano emite som e flash vermelho.
10. HP baixo pulsa visualmente.
11. Vitória/morte funcionam.
12. A+B reinicia.
