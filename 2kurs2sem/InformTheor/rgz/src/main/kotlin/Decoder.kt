package org.example

import java.io.FileReader
import kotlin.math.log2
import kotlin.math.pow

class Decoder {
    private var encodedData = listOf<Boolean>()

    fun setEncodedData(data: List<Boolean>): Decoder {
        encodedData = data
        return this
    }

    fun setEncodedDataFromString(data: String): Decoder {
        encodedData = mutableListOf<Boolean>().apply {
            for (c in data) {
                add(c == '1')
            }
        }

        return this
    }

    fun setEncodedDataFromFile(path: String): Decoder {
        return setEncodedDataFromString(FileReader(path).readText())
    }

    fun decode(): String {
        val metadata = decodeHammingCode(encodedData.subList(0, 14))
        val controlBits = metadata.subList(0, 5).toInt()
        val lengthOfElem = metadata.subList(5, 10).toInt()

        val decodedData = encodedData.subList(14, encodedData.size)
        val numSequence = decodeHammingCode(decodedData, controlBits, lengthOfElem)
        val data = decodeLzw(numSequence)

        return data
    }

    private fun decodeHammingCode(
        bitSequence: List<Boolean>,
        controlBits: Int,
        lengthOfElem: Int
    ): List<Int> {
        val blockSize: Int = (2.0.pow(controlBits) - 1).toInt()
        val buf = mutableListOf<List<Boolean>>().apply {
            for (i in 0..<(bitSequence.size / blockSize)) {
                add(bitSequence.subList(i * blockSize, i * blockSize + blockSize).toMutableList())
            }
            if (bitSequence.size % blockSize != 0) {
                add(bitSequence.subList(bitSequence.size / blockSize * blockSize, bitSequence.size).toMutableList())
            }
        }

        val decodedSequence = mutableListOf<Boolean>().apply {
            buf.forEach {
                addAll(decodeHammingCode(it))
            }
        }

        val res = mutableListOf<Int>().apply {
            for (i in 0..<decodedSequence.size / lengthOfElem) {
                add(decodedSequence.subList(i * lengthOfElem, i * lengthOfElem + lengthOfElem).toInt())
            }
        }

        return res
    }


    private fun decodeHammingCode(bitSequence: List<Boolean>): List<Boolean> {
        val block = bitSequence.toMutableList()

        // init parity bits
        var pbit = 0
        var posForCorrecting = -1
        while (1.shl(pbit) - 1 < block.size) {
            var isCount = true
            var isParity = true
            for (i in 1.shl(pbit) - 1..<block.size step 1.shl(pbit)) {
                for (j in i..<i + 1.shl(pbit)) {
                    if (j >= block.size) {
                        break
                    }
                    if (isCount) {
                        isParity = if (block[j]) !isParity else isParity
                    }
                }
                isCount = !isCount
            }
            if (!isParity) {
                posForCorrecting =
                    (if (posForCorrecting < 0) 0 else posForCorrecting) + 1.shl(pbit)
            }
            pbit++
        }

        if (posForCorrecting - 1 >= block.size) {
            throw NotCorrectedByPBitException()
        } else if (posForCorrecting >= 0) {
            block[posForCorrecting - 1] = !block[posForCorrecting - 1]
        }

        // del parity bits
        pbit = log2(block.size.toDouble()).toInt()
        while (pbit >= 0) {
            block.removeAt(1.shl(pbit) - 1)
            pbit--
        }

        return block
    }

    private fun decodeLzw(numSequence: List<Int>): String {
        val dictionary = mutableMapOf<Int, String>().apply {
            for (i in 0..<128) put(i, i.toChar().toString())
        }
        var data = numSequence.toList()
        var prevBuf = data[0].toChar().toString()
        data = data.drop(1)
        var res = prevBuf
        for (k in data) {
            var buf = ""
            if (dictionary.containsKey(k)) {
                buf = dictionary[k]!!
            } else if (k == dictionary.size) {
                buf = prevBuf + prevBuf[0]
            }
            res += buf
            try {
                dictionary[dictionary.size] = prevBuf + buf[0]
            } catch (e: StringIndexOutOfBoundsException) {
                throw NotCorrectedByPBitException()
            }
            prevBuf = buf
        }

        return res
    }
}

class NotCorrectedByPBitException : Exception()