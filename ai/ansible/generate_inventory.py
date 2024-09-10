import json
import os

# Terraform 출력 파일 읽기
with open('/home/ubuntu/jenkins/howabout/terraform-project/ai/terraform_output.json') as f:  # 파일 경로 수정
    data = json.load(f)

# EC2 인스턴스의 IP 주소 추출
ai_instance_ip = data['ai_instance_ip']['value'][0]  # 리스트에서 첫 번째 요소 선택

# SSH 사용자 이름 읽기
ssh_user = 'ubuntu'  # 직접 값 설정

# Ansible 인벤토리 파일 생성
inventory_content = f"""
[aiservers]
aiserver1 ansible_host={ai_instance_ip} ansible_user={ssh_user}
"""

# 인벤토리 파일 저장
with open('inventory.ini', 'w') as f:
    f.write(inventory_content)
