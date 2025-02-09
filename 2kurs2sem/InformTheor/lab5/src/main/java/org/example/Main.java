package org.example;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.Map;


class ArithmeticEncoder {
    private ArrayList<Map.Entry<Character, Double>> tablePrevProbabilities;


    private void setProbabilityLine(String source) {
        tablePrevProbabilities = new ArrayList<>();
        // подсчёт количества символов
        HashMap<Character, Integer> amountsOfSymbol = new HashMap<>();
        for (var symbol : source.toCharArray()) {
            amountsOfSymbol.compute(symbol, (k, v) -> v != null ? v + 1 : 1);
        }
        // подсчёт частот символов
        HashMap<Character, Double> probabilitiesOfSymbol = new HashMap<>();
        for (var pairSymbolNCounter : amountsOfSymbol.entrySet()) {
            probabilitiesOfSymbol.put(pairSymbolNCounter.getKey(), (double) pairSymbolNCounter.getValue() / source.length());
        }
        // сортировка по частоте
        for (var pairSymbolNProbability : probabilitiesOfSymbol.entrySet()) {
            int i = 0;
            while (i < tablePrevProbabilities.size() &&
                    tablePrevProbabilities.get(i).getValue() > pairSymbolNProbability.getValue())
                i++;

            tablePrevProbabilities.add(i, pairSymbolNProbability);
        }

        var temp =  tablePrevProbabilities.get(0).getValue();
        tablePrevProbabilities.get(0).setValue(0.);
        // формовка частот от 0 до 1
        for (int i = 1; i < tablePrevProbabilities.size(); i++) { // todo от 0 сделать
            double prevValue = tablePrevProbabilities.get(i - 1).getValue();
            double currValue = tablePrevProbabilities.get(i).getValue();
            tablePrevProbabilities.get(i).setValue(currValue + prevValue);
        }
    }

    private Double getHighRangeProbabilityLine(char symbol) {
        for (var pairSymbolNProbability : tablePrevProbabilities)
            if (pairSymbolNProbability.getKey() == symbol)
                return pairSymbolNProbability.getValue();

        return null;
    }

    private Double getLowRangeProbabilityLine(char symbol) {
        for (int i = 0; i < tablePrevProbabilities.size(); i++)
            if (tablePrevProbabilities.get(i).getKey() == symbol)
                if (i == 0) return 0.;
                else return tablePrevProbabilities.get(i - 1).getValue();

        return null;
    }


    public void initVariables() {
        setProbabilityLine("SSSEHHHH");
    }

    public Double encode(String text) throws NullPointerException {
        var sb = new StringBuilder();

        double low = 0;
        double high = 1;
        for (var symbol : text.toCharArray())
            try {
                low += (high - low) * getLowRangeProbabilityLine(symbol);
                high += (high - low) * getHighRangeProbabilityLine(symbol);
            } catch (NullPointerException e) {
                e.fillInStackTrace();
                return null;
            }

        return (low + high) / 2;
    }

    public String decode(Double encodedValue) {
        var sb = new StringBuilder();

        for (var pairSymbolNProbability : tablePrevProbabilities) {
            if (encodedValue > pairSymbolNProbability.getValue()) {
                sb.append(pairSymbolNProbability.getKey());
                encodedValue -= pairSymbolNProbability.getValue();
            }
        }

        code=(code-RangeLow(x))/(RangeHigh(x)-RangeLow(x))
        //, где code – текущее значение кода.

        return sb.toString();
    }
}

public class Main {
    public static void main(String[] args) {
        new ArithmeticEncoder().initVariables();
    }
}