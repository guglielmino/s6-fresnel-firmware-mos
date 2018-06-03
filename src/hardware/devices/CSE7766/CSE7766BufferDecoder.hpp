//
// Created by Fabrizio Guglielmino on 02/06/18.
//

#ifndef S6FRESNEL_VALUESDECODER_HPP
#define S6FRESNEL_VALUESDECODER_HPP

#define CSE7766_V1R                     1.0     // 1mR current resistor
#define CSE7766_V2R                     1.0     // 1M voltage resistor

class CSE7766BufferDecoder {
public:
    enum ErrorCodes {
        SUCCESS,
        NOT_CALIBRATED,
        WRONG_CHECKSUM,
        ABNORMAL_COEFFICIENT_STORAGE_AREA,
        POWER_CYCLE_EXCEEDED_RANGE,
        CURRENT_CYCLE_EXCEEDED_RANGE,
        VOLTAGE_CYCLE_EXCEEDED_RANGE

    };

    struct Values {
        double_t activePower;
        double_t voltage;
        double_t current;
        double_t energy;
    };

    struct ProcessResult {
        struct Values v;
        ErrorCodes error;
    };

private:
    double_t ratioV = 1.0;
    double_t ratioC = 1.0;
    double_t ratioP = 1.0;


    struct Coefficients {
        unsigned long coefV;
        unsigned long coefC;
        unsigned long coefP;
    };

    bool checksum(const uint8_t *data) {
        unsigned char checksum = 0;
        for (unsigned char i = 2; i < 23; i++) {
            checksum += data[i];
        }
        return checksum == data[23];
    }

    ErrorCodes checkBuffer(const uint8_t *data) {
        ErrorCodes ret = SUCCESS;
        if (0xAA == data[0]) {
            return NOT_CALIBRATED;
        }

        if ((data[0] & 0xFC) > 0xF0) {
            if (0xF1 == (data[0] & 0xF1)) ret = ABNORMAL_COEFFICIENT_STORAGE_AREA;
            if (0xF2 == (data[0] & 0xF2)) ret = POWER_CYCLE_EXCEEDED_RANGE;
            if (0xF4 == (data[0] & 0xF4)) ret = CURRENT_CYCLE_EXCEEDED_RANGE;
            if (0xF8 == (data[0] & 0xF8)) ret = VOLTAGE_CYCLE_EXCEEDED_RANGE;
        }

        return ret;
    }

    Coefficients calibCoeffs(const uint8_t *data) {
        Coefficients ret;
        ret.coefV = (data[2] << 16 | data[3] << 8 | data[4]);
        ret.coefC = (data[8] << 16 | data[9] << 8 | data[10]);
        ret.coefP = (data[14] << 16 | data[15] << 8 | data[16]);

        return ret;
    }

    double_t voltage(Coefficients coeffs, const uint8_t *data) {
        uint8_t adj = data[20];

        // Calculate voltage
        double_t voltage = 0;
        if ((adj & 0x40) == 0x40) {
            unsigned long voltage_cycle = data[5] << 16 | data[6] << 8 | data[7];        // 817
            voltage = ratioV * coeffs.coefV / voltage_cycle / CSE7766_V2R;               // 190700 / 817 = 233.41
        }

        return voltage;
    }

    double_t activePower(Coefficients coeffs, const uint8_t *data) {
        double_t active = 0;
        uint8_t adj = data[20];

        if ((adj & 0x10) == 0x10) {
            if ((data[0] & 0xF2) != 0xF2) {
                unsigned long power_cycle = data[17] << 16 | data[18] << 8 | data[19];
                active = ratioP * coeffs.coefP / power_cycle / CSE7766_V1R / CSE7766_V2R;
            }
        }

        return active;
    }

    double_t current(Coefficients coeffs, const uint8_t *data) {
        double_t current = 0;
        uint8_t adj = data[20];

        if ((adj & 0x20) == 0x20) {
            unsigned long current_cycle = data[11] << 16 | data[12] << 8 | data[13];
            current = ratioC * coeffs.coefC / current_cycle / CSE7766_V1R;
        }

        return current;
    }

    double_t energy(Coefficients coeffs, const uint8_t *data) {
        double_t energy;

        unsigned int difference;
        static unsigned int cf_pulses_last = 0;
        unsigned int cf_pulses = data[21] << 8 | data[22];
        if (0 == cf_pulses_last) cf_pulses_last = cf_pulses;
        if (cf_pulses < cf_pulses_last) {
            difference = cf_pulses + (0xFFFF - cf_pulses_last) + 1;
        } else {
            difference = cf_pulses - cf_pulses_last;
        }
        energy += difference * (float) coeffs.coefP / 1000000.0;
        cf_pulses_last = cf_pulses;

        return energy;
    }

public:

    ProcessResult process(const uint8_t *data) {
        ProcessResult r;

        if (!checksum(data)) {
            r.error = WRONG_CHECKSUM;
            return r;
        }

        r.error = checkBuffer(data);
        if (r.error == SUCCESS) {
            Coefficients coeffs = calibCoeffs(data);
            r.v.voltage = voltage(coeffs, data);
            r.v.activePower = activePower(coeffs, data);
            r.v.current = current(coeffs, data);
            r.v.energy = energy(coeffs, data);
        }
        return r;
    }
};

#endif //S6FRESNEL_VALUESDECODER_HPP
