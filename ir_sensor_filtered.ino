// ir_sensor_filtered() - 설명 영상용 구현 (과제 19C08_14)
// ※ 수업 예제코드(19_example_1.ino)의 원본 함수와 다르면,
//    영상에서는 반드시 "본인이 실제로 분석한 원본 코드"를 기준으로 설명하세요.

#define PIN_IR      A0
#define IR_MAX_N    100   // 슬라이드 조건: 0 < n < 100

// n번 측정 -> 오름차순 정렬 -> position 위치의 값 반환
// position: 0 (최솟값) ~ 0.5 (중앙값) ~ 1 (최댓값)
// verbose : 0 정상, 1 동작 과정 출력, 2 측정 시간 출력
unsigned int ir_sensor_filtered(unsigned int n, float position, int verbose) {
  unsigned int ir_val[IR_MAX_N];          // 측정값 버퍼 (2 byte x 100 = 200 byte)
  unsigned long t0 = micros();

  // 입력값 보정
  if (n < 1) n = 1;
  if (n > IR_MAX_N - 1) n = IR_MAX_N - 1;
  if (position < 0.0) position = 0.0;
  if (position > 1.0) position = 1.0;

  // 1) n회 측정 : O(n)
  for (unsigned int i = 0; i < n; i++)
    ir_val[i] = analogRead(PIN_IR);

  if (verbose == 1) {
    Serial.print("IR: ");
    for (unsigned int i = 0; i < n; i++) { Serial.print(ir_val[i]); Serial.print(' '); }
  }

  // 2) 오름차순 정렬 (삽입 정렬, 제자리) : 최악 O(n^2)
  for (unsigned int i = 1; i < n; i++) {
    unsigned int key = ir_val[i];
    int j = (int)i - 1;
    while (j >= 0 && ir_val[j] > key) {
      ir_val[j + 1] = ir_val[j];
      j--;
    }
    ir_val[j + 1] = key;
  }

  // 3) position -> 인덱스 (반올림). n=5, 0.5 -> 2 / 0 -> 0 / 1 -> n-1
  unsigned int ret_idx = (unsigned int)(position * (n - 1) + 0.5);

  if (verbose == 1) {
    Serial.print("=> ");
    for (unsigned int i = 0; i < n; i++) { Serial.print(ir_val[i]); Serial.print(' '); }
    Serial.print(":: "); Serial.println(ir_val[ret_idx]);
  }
  if (verbose == 2) {
    Serial.print("n="); Serial.print(n);
    Serial.print(", time(us)="); Serial.println(micros() - t0);
  }
  return ir_val[ret_idx];
}

void setup() {
  Serial.begin(115200);
}

void loop() {
  while (Serial.available() == 0) ;
  Serial.read();                          // <Enter> 대기
  ir_sensor_filtered(5, 0.5, 1);          // 동작 과정 출력
  ir_sensor_filtered(50, 0.5, 2);         // 실행 시간 출력
}
