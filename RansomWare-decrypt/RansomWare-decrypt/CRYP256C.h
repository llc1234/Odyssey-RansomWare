#pragma once


class CRYP256C {
public:
    std::string filename;
    std::string key;
    std::vector<std::string> all_keys;
    std::vector<std::vector<uint8_t>> data_list;

    std::vector<uint8_t> switch_bytes_array = {
        0x38, 0x39, 0x3a, 0x3b, 0x0c, 0x0d, 0x0e, 0x0f,
        0x3c, 0x3d, 0x3e, 0x3f, 0x28, 0x29, 0x2a, 0x2b,
        0x2c, 0x2d, 0x2e, 0x2f, 0x30, 0x31, 0x32, 0x33,
        0x08, 0x09, 0x0a, 0x0b, 0x74, 0x75, 0x76, 0x77,
        0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
        0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
        0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27,
        0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f,
        0x34, 0x35, 0x36, 0x37, 0x70, 0x71, 0x72, 0x73,
        0xe0, 0xe1, 0xe2, 0xe3, 0xe4, 0xe5, 0xe6, 0xe7,
        0xc8, 0xc9, 0xca, 0xcb, 0xcc, 0xcd, 0xce, 0xcf,
        0xe8, 0xe9, 0xea, 0xeb, 0xec, 0xed, 0xee, 0xef,
        0xd0, 0xd1, 0xd2, 0xd3, 0xd4, 0xd5, 0xd6, 0xd7,
        0xb0, 0xb1, 0xb2, 0xb3, 0xb4, 0xb5, 0xb6, 0xb7,
        0xf0, 0xf1, 0xf2, 0xf3, 0xf4, 0xf5, 0xf6, 0xf7,
        0xf8, 0xf9, 0xfa, 0xfb, 0xfc, 0xfd, 0xfe, 0xff,
        0xb8, 0xb9, 0xba, 0xbb, 0xbc, 0xbd, 0xbe, 0xbf,
        0x88, 0x89, 0x8a, 0x8b, 0x8c, 0x8d, 0x8e, 0x8f,
        0xd8, 0xd9, 0xda, 0xdb, 0xdc, 0xdd, 0xde, 0xdf,
        0x78, 0x79, 0x7a, 0x7b, 0x7c, 0x7d, 0x7e, 0x7f,
        0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57,
        0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67,
        0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97,
        0x68, 0x69, 0x6a, 0x6b, 0x6c, 0x6d, 0x6e, 0x6f,
        0x98, 0x99, 0x9a, 0x9b, 0x9c, 0x9d, 0x9e, 0x9f,
        0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87,
        0xa8, 0xa9, 0xaa, 0xab, 0xac, 0xad, 0xae, 0xaf,
        0xc0, 0xc1, 0xc2, 0xc3, 0xc4, 0xc5, 0xc6, 0xc7,
        0x58, 0x59, 0x5a, 0x5b, 0x5c, 0x5d, 0x5e, 0x5f,
        0x48, 0x49, 0x4a, 0x4b, 0x4c, 0x4d, 0x4e, 0x4f,
        0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47,
        0xa0, 0xa1, 0xa2, 0xa3, 0xa4, 0xa5, 0xa6, 0xa7
    };

    uint64_t hash128(const std::string& input_string) {
        uint32_t h1 = 0x12345678;
        uint32_t h2 = 0x9abcdef0;

        uint32_t prime1 = 0x45d9f3b;
        uint32_t prime2 = 0x41c6ce57;

        for (size_t i = 0; i < input_string.size(); i += 8) {
            std::string chunk = input_string.substr(i, 8);
            uint64_t chunk_value = 0;

            for (char c : chunk) {
                chunk_value = (chunk_value << 8) + static_cast<uint8_t>(c);
            }

            h1 = (h1 ^ chunk_value) * prime1;
            h2 = (h2 ^ chunk_value) * prime2;

            h1 = h1 & 0xffffffff;
            h2 = h2 & 0xffffffff;
        }

        uint64_t final_hash = (static_cast<uint64_t>(h1) << 32) | h2;
        std::ostringstream oss;
        oss << std::hex << std::setw(16) << std::setfill('0') << final_hash;
        return final_hash;
    }

    void split_data() {
        std::ifstream infile(filename, std::ios::binary);
        std::vector<uint8_t> buffer(std::istreambuf_iterator<char>(infile), {});

        for (size_t i = 0; i < buffer.size(); i += 64) {
            std::vector<uint8_t> chunk(buffer.begin() + i, buffer.begin() + std::min(buffer.size(), i + 64));
            data_list.push_back(chunk);
        }
    }

    std::vector<uint8_t> XORGate(const std::vector<uint8_t>& message) {
        std::vector<uint8_t> encrypted_message;
        size_t key_length = key.size();

        for (size_t i = 0; i < message.size(); ++i) {
            uint8_t encrypted_byte = message[i] ^ static_cast<uint8_t>(key[i % key_length]);
            encrypted_message.push_back(encrypted_byte);
        }

        return encrypted_message;
    }

    std::vector<uint8_t> switch_bytes(const std::vector<uint8_t>& input_bytes) {
        std::vector<uint8_t> output_bytes;

        for (uint8_t byte : input_bytes) {
            output_bytes.push_back(switch_bytes_array[byte]);
        }

        return output_bytes;
    }

    std::vector<uint8_t> reverse_switch_bytes(const std::vector<uint8_t>& input_bytes) {
        std::vector<uint8_t> reverse_switch(256);

        for (size_t i = 0; i < switch_bytes_array.size(); ++i) {
            reverse_switch[switch_bytes_array[i]] = static_cast<uint8_t>(i);
        }

        std::vector<uint8_t> output_bytes;

        for (uint8_t byte : input_bytes) {
            output_bytes.push_back(reverse_switch[byte]);
        }

        return output_bytes;
    }

    void start_mix_up() {
        std::ofstream outfile(filename, std::ios::binary);

        for (const auto& pp : data_list) {
            key = all_keys[0];
            auto text = XORGate(pp);
            text = switch_bytes(text);
            key = all_keys[1];
            text = XORGate(text);
            text = switch_bytes(text);
            key = all_keys[2];
            text = XORGate(text);
            text = switch_bytes(text);
            key = all_keys[3];
            text = XORGate(text);

            outfile.write(reinterpret_cast<const char*>(text.data()), text.size());
        }
    }

    void start_mix_down() {
        std::ofstream outfile(filename, std::ios::binary);

        for (const auto& pp : data_list) {
            key = all_keys[3];
            auto text = XORGate(pp);
            text = reverse_switch_bytes(text);
            key = all_keys[2];
            text = XORGate(text);
            text = reverse_switch_bytes(text);
            key = all_keys[1];
            text = XORGate(text);
            text = reverse_switch_bytes(text);
            key = all_keys[0];
            text = XORGate(text);

            outfile.write(reinterpret_cast<const char*>(text.data()), text.size());
        }
    }

    void encrypt() {
        split_data();
        start_mix_up();

        int i = std::rename(filename.c_str(), (filename + ".CRYP256C").c_str());
    }

    void decrypt() {
        split_data();
        start_mix_down();

        std::string new_filename = filename.substr(0, filename.size() - 9);
        int I = std::rename(filename.c_str(), new_filename.c_str());
    }

    void en_of_de() {
        if (filename.size() > 9 && filename.substr(filename.size() - 9) == ".CRYP256C") {
            decrypt();
        }
        else {
            encrypt();
        }
    }

    void key_generator(const std::string& start_seed) {
        std::string k = hash128_string(start_seed);

        for (int i = 0; i < 4; ++i) {
            k = hash128_string(k);
            all_keys.push_back(k);
        }
    }

    std::string hash128_string(const std::string& input_string) {
        std::ostringstream oss;
        oss << std::hex << std::setw(16) << std::setfill('0') << hash128(input_string);
        return oss.str();
    }

    void start(std::string name_of_file, char* key) {

        filename = name_of_file;
        key_generator(hash128_string(key));
        en_of_de();
    }
};
