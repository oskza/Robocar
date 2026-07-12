class OtaService {
private:
    bool _started;
public:
    OtaService();
    bool begin(const char *hostname);
    void update();
    bool isStarted() const;
};
