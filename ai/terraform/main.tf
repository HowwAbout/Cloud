provider "aws" {
  region = "ap-northeast-2"
}

variable "instance_count" {
  default = 1
}

variable "instance_type" {
  default = "t3.medium"
}

resource "aws_vpc" "ai_vpc" {
  cidr_block = "192.172.0.0/16"

  tags = {
    Name = "howabout-ai-vpc"
  }
}

resource "aws_subnet" "ai_subnet" {
  vpc_id                  = aws_vpc.ai_vpc.id
  cidr_block              = "192.172.1.0/24"
  availability_zone       = "ap-northeast-2a"
  map_public_ip_on_launch = true

  tags = {
    Name = "howabout-ai-subnet"
  }
}

resource "aws_internet_gateway" "ai_igw" {
  vpc_id = aws_vpc.ai_vpc.id

  tags = {
    Name = "howabout-ai-igw"
  }
}

resource "aws_route_table" "ai_route" {
  vpc_id = aws_vpc.ai_vpc.id

  route {
    cidr_block = "0.0.0.0/0"
    gateway_id = aws_internet_gateway.ai_igw.id
  }

  tags = {
    Name = "howabout-ai-route"
  }
}

resource "aws_route_table_association" "ai_route_association" {
  subnet_id      = aws_subnet.ai_subnet.id
  route_table_id = aws_route_table.ai_route.id
}

resource "aws_security_group" "ai_sg" {
  vpc_id = aws_vpc.ai_vpc.id

  ingress {
    from_port   = 22
    to_port     = 22
    protocol    = "tcp"
    cidr_blocks = ["0.0.0.0/0"]
  }

  ingress {
    from_port   = 9000
    to_port     = 9000
    protocol    = "tcp"
    cidr_blocks = ["0.0.0.0/0"]
  }

  ingress {
    from_port   = 8000
    to_port     = 8000
    protocol    = "tcp"
    cidr_blocks = ["0.0.0.0/0"]
  }

  ingress {
    from_port   = 8080
    to_port     = 8080
    protocol    = "tcp"
    cidr_blocks = ["0.0.0.0/0"]
  }

  ingress {
    from_port   = 8081
    to_port     = 8081
    protocol    = "tcp"
    cidr_blocks = ["0.0.0.0/0"]
  }

  ingress {
    from_port   = 3000
    to_port     = 3000
    protocol    = "tcp"
    cidr_blocks = ["0.0.0.0/0"]
  }

  ingress {
    from_port   = 80
    to_port     = 80
    protocol    = "tcp"
    cidr_blocks = ["0.0.0.0/0"]
  }

  ingress {
    from_port   = 443
    to_port     = 443
    protocol    = "tcp"
    cidr_blocks = ["0.0.0.0/0"]
  }

  ingress {
    from_port   = 3306
    to_port     = 3306
    protocol    = "tcp"
    cidr_blocks = ["0.0.0.0/0"]
  }

  ingress {
    from_port   = 6379  # 확인 필요 (일반적으로 Redis의 포트)
    to_port     = 6379
    protocol    = "tcp"
    cidr_blocks = ["0.0.0.0/0"]
  }

  egress {
    from_port   = 0
    to_port     = 0
    protocol    = "-1"
    cidr_blocks = ["0.0.0.0/0"]
  }

  tags = {
    Name = "howabout-ai-sg"
  }
}

resource "aws_instance" "web" {
  count         = var.instance_count
  ami           = "ami-062cf18d655c0b1e8"
  instance_type = var.instance_type
  key_name      = "aws1"
  subnet_id     = aws_subnet.ai_subnet.id
  vpc_security_group_ids = [aws_security_group.ai_sg.id]

  user_data = <<-EOF
#!/bin/bash
mkdir -p /home/ubuntu/.ssh
echo "ssh-rsa AAAAB3NzaC1yc2EAAAADAQABAAACAQCiEoTwLPsz+ggorBYQMVKZoQPl1+k9pSuwZwHesIbTEp9SjVsu5nQYOii5LDrNFbcC5YGJf8j0rkBlG8US9wf9MIfUitoXjsB4XSTf2CdSVwzhS3nxD66+Z0hH1HXrw4QQhu6vCPSTkBycPt+uNn/p9MX3E6ji8XMq5L5630zFYyxGLkgsramljL4DBrGgCjwU0EJyM8AjsdNqAiwY4UjQAZi+2Ar8ka0Jp0xvd6g1ScyH8ESFJmIt1OnZ2h109mOLi8ARUneBII6f6USZkU+gHFlrfUjCRWOQVetpvdHcvSSW2oMOwR8nTrxYgL/dlnsqev2nMpEkrBC3BLUlA14+sW9fDmsb3o0/lGpNUza+u6D7VEXqN3wj7ap9ahhN11WLcRTi1nHjJfAy5LZSoZnDBh9/1Y9tTIUuCE2OZTnjx9JF8R1bBgmpEGZ750Izz6kwxoIL0iVEFj/rbQVlG3DNoDx6wPhhqgADntJrWVZoTIu+Q21NBK//uPa5zY9WiW8BMniaoqoj4mvPDr4R9ATT6850PlBf98CAKzimdJIkWF09q+Gz+Cm3M6JpBZAoNZo0zN1JQWT0kCAToLecUrdM7h/N66e74teNSxOBzHNVYUCEW3ZrEye817N/iAL6/WxToQ3OSL1klO7KMyOq2EtvEboIxBaBt6Lv4ccOIN8Kbw== uniti0903@naver.com" >> /home/ubuntu/.ssh/authorized_keys
chown ubuntu:ubuntu /home/ubuntu/.ssh/authorized_keys
chmod 600 /home/ubuntu/.ssh/authorized_keys
EOF

  tags = {
    Name = "howabout-ai"
  }
}

output "instance_ips" {
  value = aws_instance.web.*.public_ip
}
